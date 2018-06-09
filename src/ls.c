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
#include <sys/types.h>
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
#include "../include/strvec.h"

#include "../include/ls.h"

static int display_loc_info( const char * path, const char * loc, size_t flags, const int max_file_len );
static void get_file_size_formatted( const size_t size, char * res );
static struct str_vec * generate_file_str_vec( DIR * dir, const size_t flags );
static int get_max_file_len( const struct str_vec * locs );
static void sort_list_dirs_first( struct str_vec * locs );

// Length to accomodate the icons and tabs in non -l mode.
// Should be = total length of icons, spaces and the ending /
// + 14 ( for the terminal color codes )
#define EXTRA_ITEM_LEN 20

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

	DIR * dir = opendir( finaldir );
	if( dir == NULL ) {
		display_err( "{p}Unable to open directory{0}: {r}%s{0}\n", finaldir );
		return LOC_NOT_OPENED;
	}
	struct str_vec * locs = generate_file_str_vec( dir, flags );
	closedir( dir );

	if( locs == NULL ) return STR_VEC_INIT_FAIL;

	int max_len_in_files = get_max_file_len( locs ) + EXTRA_ITEM_LEN;
	// - 14 to compensate for the terminal color codes
	// The color codes are not displayed but do account for padding ( i guess ),
	// and hence must be manually handled
	int max_items_per_line = ( int ) ws->ws_col / ( max_len_in_files - 14 );
	if( max_items_per_line < 1 ) max_items_per_line = 1;

	int items_per_line_ctr = 0;

	for( int i = 0; i < ( int )str_vec_get_count( locs ); ++i ) {
		// don't pad shift if the item is last on the line since pad won't be needed
		// in that case anyway
		int pad_shift = items_per_line_ctr == ( max_items_per_line - 1 ) ? 0 : max_len_in_files;
		int temp_res = display_loc_info( finaldir, str_vec_get( locs, i ), flags, pad_shift );
		if( temp_res != 0 ) {
			str_vec_delete( & locs );
			return temp_res;
		}
		if( !( flags & OPT_L ) ) {
			++items_per_line_ctr;
			if( items_per_line_ctr == max_items_per_line ) {
				display( "\n" );
				items_per_line_ctr = 0;
			}
		}
	}

	str_vec_delete( & locs );

	if( !( flags & OPT_F ) )
		display( "\n" );

	return SUCCESS;
}


// Colors for various types are:
// blue - directories
// green - files
// yellow - symlinks
static int display_loc_info( const char * path, const char * loc, size_t flags, const int max_file_len )
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
		if( S_ISDIR( st.st_mode ) ) {
			display_padded( max_file_len, "{b}%s  %s/{0}", icon, loc );
			return SUCCESS;
		}

		if( S_ISLNK( st.st_mode ) ) {
			display_padded( max_file_len, "{y}%s  %s{0}", "\uf482", loc );
			return SUCCESS;
		}

		display_padded( max_file_len, "{g}%s  %s{0}", icon, loc );
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

static struct str_vec * generate_file_str_vec( DIR * dir, const size_t flags )
{
	struct dirent * di;
	struct str_vec * locs = str_vec_create();
	struct str_vec * dir_locs;
	if( flags & OPT_S ) dir_locs = str_vec_create();

	if( locs == NULL ) {
		display_err( "{p}Unable to create string vector {r}locs{0}\n" );
		return NULL;
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

		if( ( flags & OPT_S ) && di->d_type == DT_DIR ) str_vec_add( dir_locs, di->d_name );
		else str_vec_add( locs, di->d_name );
	}

	str_vec_sort( locs );

	if( flags & OPT_S ) {
		str_vec_sort( dir_locs );
		str_vec_add_vec( dir_locs, locs );
		str_vec_delete( & locs );
		locs = dir_locs;
		dir_locs = NULL;
	}

	return locs;
}

static int get_max_file_len( const struct str_vec * locs )
{
	int max_len = 0;
	for( int i = 0; i < ( int )locs->count; ++i ) {
		int len = strlen( str_vec_get( locs, i ) );
		if( max_len < len ) max_len = len;
	}

	return max_len;
}
