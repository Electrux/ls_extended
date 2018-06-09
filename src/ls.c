/*
	Copyright (c) 2018, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <grp.h>
#include <pwd.h>
#include <time.h>

#include "../include/core.h"
#include "../include/cmdopts.h"
#include "../include/icons.h"

static int display_loc_info( const struct winsize * ws, const char * path, const char * loc, size_t flags );
static void get_file_ext( const char * file, char * ext );
static void get_file_size_formatted( const size_t size, char * res );

int ls( const struct winsize * ws, const char * loc, size_t flags )
{
	char finaldir[ 1024 ];
	if( strcmp( loc, "\0" ) == 0 ) strcpy( finaldir, "." );
	else strcpy( finaldir, loc );

	if( finaldir[ strlen( finaldir ) - 1 ] != '/' ) {
		strcat( finaldir, "/" );
	}

	// remove -l dependent flags if -l is not present
	if( !( flags & OPT_L ) ) {
		flags &= ~OPT_G;
		flags &= ~OPT_H;
		flags &= ~OPT_I;
	}

	struct dirent * di;
	DIR * dir = opendir( finaldir );
	/*if( dir == NULL ) {
		display_err( "{p}Unable to open directory{0}: {r}%s{0}\n", finaldir );
		return LOC_NOT_OPENED;
	}

	int max_entity_len;
	while( ( di = readdir( dir ) ) != NULL ) {
		int tmp_len = strlen( di->d_name );
		if( tmp_len > max_entity_len )
			max_entity_len = tmp_len;
	}
	closedir( dir );

	dir = opendir( finaldir );
	*/
	if( dir == NULL ) {
		display_err( "{p}Unable to open directory{0}: {r}%s{0}\n", finaldir );
		return LOC_NOT_OPENED;
	}

	while( ( di = readdir( dir ) ) != NULL ) {
		if( flags & OPT_D || flags & OPT_F ) {
			if( di->d_type == DT_DIR && !( flags & OPT_D ) )
				continue;
			if( di->d_type != DT_DIR && !( flags & OPT_F ) )
				continue;
		}
		if( strncmp( di->d_name, ".", 1 ) == 0 || strncmp( di->d_name, "..", 2 ) == 0 ) {
			if( ( strcmp( di->d_name, "." ) == 0 || strcmp( di->d_name, ".." ) == 0 ) && !( flags & OPT_A ) )
				continue;
			else if( !( flags & OPT_A ) && !( flags & OPT_CAPS_A ) )
				continue;
		}

		int temp_res = display_loc_info( ws, finaldir, di->d_name, flags );
		if( temp_res != 0 ) return temp_res;
	}

	closedir( dir );
	if( !( flags & OPT_F ) )
		display( "\n" );
	return SUCCESS;
}


// Colors for various types are:
// blue - directories
// green - files
// yellow - symlinks
static int display_loc_info( const struct winsize * ws, const char * path, const char * loc, size_t flags )
{
	struct stat st;
	char full_path[ 1024 ];
	strcpy( full_path, path );
	strcat( full_path, loc );
	int temp_res = lstat( full_path, & st );
	if( temp_res != 0 ) {
		display( "{p}Something went wrong in fetching information of {r}%s{0}, {p}error{0}: {s}%d\n", full_path, errno );
		return errno;
	}

	const char * icon = S_ISDIR( st.st_mode ) ? get_dir_icon( loc ) : get_file_icon( loc );

	if( !( flags & OPT_L ) ) {
		// manages columns of terminal to determine when to put the info on a new line
		// only for when -l is NOT used
		static uint16_t col_ctr = 0;

		if( col_ctr + strlen( loc ) + 16 >= ws->ws_col ) {
			display( "\n" );
			col_ctr = 0;
		}
		col_ctr += strlen( loc ) + 8;

		if( S_ISDIR( st.st_mode ) ) {
			display( "{b}%s  %s/{0}\t", icon, loc );
			return SUCCESS;
		}

		if( S_ISLNK( st.st_mode ) ) {
			display( "{y}%s  %s{0}\t", "\uf482", loc );
			return SUCCESS;
		}

		display( "{g}%s  %s{0}\t", icon, loc );
		return SUCCESS;
	}

	// Now starts the ls with -l option

	display( " " );

	// inode number
	if( flags & OPT_I ) {
		display_padded( 12, "%llu", st.st_ino );
	}

	// directory/link/file
	if( S_ISDIR( st.st_mode ) ) display( "{b}d" );
	else if( S_ISLNK( st.st_mode ) ) display( "{b}l" );
	else display( "{0}-" );

	// permissions
	display( ( st.st_mode & S_IRUSR ) ? "{m}r" : "{0}-" );
	display( ( st.st_mode & S_IWUSR ) ? "{c}w" : "{0}-" );
	display( ( st.st_mode & S_IXUSR ) ? "{r}x" : "{0}-" );
	display( ( st.st_mode & S_IRGRP ) ? "{m}r" : "{0}-" );
	display( ( st.st_mode & S_IWGRP ) ? "{c}w" : "{0}-" );
	display( ( st.st_mode & S_IXGRP ) ? "{r}x" : "{0}-" );
	display( ( st.st_mode & S_IROTH ) ? "{m}r" : "{0}-" );
	display( ( st.st_mode & S_IWOTH ) ? "{c}w" : "{0}-" );
	display( ( st.st_mode & S_IXOTH ) ? "{r}x" : "{0}-" );

	// number of links
	if( flags & OPT_I ) display( "\t" );
	display( "\t{0}%d\t", st.st_nlink );

	// user and group
	struct passwd * usr = getpwuid( st.st_uid );
	struct group * grp = getgrgid( st.st_gid );

	if( usr == NULL ) display_padded( 14, "{g}%d", st.st_uid );
	else display_padded( 15, "{g}%s", usr->pw_name );

	if( grp == NULL ) display( "\t{g}%d", st.st_gid );
	else display( "\t{g}%s", grp->gr_name );

	// file size
	if( flags & OPT_H ) {
		char size[ 30 ];
		get_file_size_formatted( st.st_size, size );
		display( "\t{y}%s", size );
	}
	else {
		display( "\t{y}%lluB", st.st_size );
	}

	// last modified time
	char mtime[ 30 ];
	strftime( mtime, 30, "%h %e %H:%M", gmtime( & st.st_mtime ) );
	display( "\t{w}%s", mtime );

	// file/folder name
	if( S_ISDIR( st.st_mode ) ) display( "\t{b}%s  %s", icon, loc );
	else if( S_ISLNK( st.st_mode ) ) display( "\t{y}%s  %s" ,"\uf482", loc );
	else display( "\t{g}%s  %s", icon, loc );

	// link info if it is a link
	if( S_ISLNK( st.st_mode ) ) {
		char buf[ 2048 ];
		ssize_t len;
		if( ( len = readlink( full_path, buf, sizeof( buf ) - 1 ) ) != -1 ) {
			buf[ len ] = '\0';
			display( " {m}-> {c}%s", buf );
		}
	}

	display( "{0}\n" );
	return SUCCESS;
}

static void get_file_size_formatted( const size_t size, char * res )
{
	float size_res = ( float )size;
	if( size < 1024 ) {
		sprintf( res, "%zu", size );
		strcat( res, "B" );
		return;
	}

	int div_count = 0;
	while( size_res > 1024 && div_count < 3 ) {
		size_res /= 1024;
		++div_count;
	}

	snprintf( res, 30, "%.1f", size_res );

	if( div_count == 1 ) strcat( res, "K" );
	else if( div_count == 2 ) strcat( res, "M" );
	else if( div_count == 3 ) strcat( res, "G" );
}
