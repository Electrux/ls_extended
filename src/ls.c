/*
	Copyright (c) 2018, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#if defined __linux__ && defined __GNUC__ && !defined __clang__
#define _DEFAULT_SOURCE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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
static int get_stats( const char * path, struct stat_info * stats );
static void format_file_size( const size_t size, char * res );
static struct str_vec * generate_file_str_vec( DIR * dir, const size_t flags );
static int get_max_file_len( struct str_vec * locs );
static void sort_list_dirs_first( struct str_vec * locs );

// Length to accomodate the icons and tabs in non -l mode.
// Should be = total length of icons ( 1 ), spaces ( 1 ), the ending / ( 1 ), and null terminator ( 1 )
// + 14 ( for the terminal color codes )
#define EXTRA_ITEM_LEN 18

int ls( const struct winsize * ws, const char * loc, size_t flags, int loc_count )
{
	const size_t loc_size = 1024;
	char final_loc[ loc_size ];

	if((strlen(loc) + 1) > loc_size) {
		return EXIT_FAILURE;
	}

	if( strcmp( loc, "\0" ) == 0 ) strcpy( final_loc, "." );
	else strcpy( final_loc, loc );

	// remove -l dependent flags if -l is not present
	if( !( flags & OPT_L ) ) {
		flags &= ~OPT_G;
		flags &= ~OPT_H;
		flags &= ~OPT_I;
	}

	struct stat tmp_st;
	int tmp_st_res = stat( final_loc, & tmp_st );
	if( tmp_st_res != 0 ) {
		display( "{p}Something went wrong in fetching information of {r}%s{0}, {p}error{0}: {s}%d\n", final_loc, errno );
		return errno;
	}
	if( !S_ISDIR( tmp_st.st_mode ) ) {
		int res = display_loc_info( NULL, final_loc, flags, 0 );
		if( !( flags & OPT_L ) ) display( "\n" );
		return res;
	}

	if( final_loc[ strlen( final_loc ) - 1 ] != '/' ) {
		strcat( final_loc, "/" );
	}

	DIR * dir = opendir( final_loc );
	if( dir == NULL ) {
		display_err( "{p}Unable to open directory{0}: {r}%s{0}\n", final_loc );
		return LOC_NOT_OPENED;
	}
	struct str_vec * locs = generate_file_str_vec( dir, flags );
	closedir( dir );

	if( locs == NULL ) return STR_VEC_INIT_FAIL;

	int max_len_in_files = get_max_file_len( locs ) + EXTRA_ITEM_LEN;
	// -1 for the first space on each line when not in -l mode
	// -12 to compensate for the terminal color codes
	// The color codes are not displayed but do account for padding ( i guess ),
	// and hence must be manually handled
	int max_items_per_line = ( int ) ( ws->ws_col - 1 ) / ( max_len_in_files - 12 );
	if( ( flags & OPT_1 ) || max_items_per_line < 1 ) max_items_per_line = 1;

	int items_per_line_ctr = 0;

	if( loc_count > 1 ) {
		display( "{bw}%s{0}:\n", final_loc );
	}

	for( int i = 0; i < ( int )str_vec_get_count( locs ); ++i ) {
		if( !( flags & OPT_L ) && items_per_line_ctr == 0 ) display( " " );
		// don't pad shift if the item is last on the line since pad won't be needed
		// in that case anyway
		int pad_shift = items_per_line_ctr == ( max_items_per_line - 1 ) ? 0 : max_len_in_files;
		int temp_res = display_loc_info( final_loc, str_vec_get( locs, i ), flags, pad_shift );
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

	if( !( flags & OPT_L ) ) display( "\n" );
	return SUCCESS;
}


// Colors for various types are:
// blue - directories
// green - files
// yellow - symlinks
static int display_loc_info( const char * path, const char * loc, size_t flags, const int max_file_len )
{
	struct stat_info stats;
	stats.lnk_is_dead = false;
	stats.lnk_jumps = 0;
	strcpy( stats.lnk_loc, "\0" );

	char full_path[ 1024 ];
	if( path != NULL ) {
		strcpy( full_path, path );
		strcat( full_path, loc );
	}
	else {
		strcpy( full_path, loc );
	}

	int temp_res = get_stats( full_path, & stats );
	if( temp_res != 0 ) {
		display( "{p}Something went wrong in fetching information of {r}%s{0}, {p}error{0}: {s}%d\n", full_path, errno );
		return errno;
	}

	char icon[ 10 ];
	if( S_ISDIR( stats.lnk_st.st_mode ) ) strcpy( icon, get_dir_icon( loc, S_ISLNK( stats.st.st_mode ) ) );
	else strcpy( icon, get_file_icon( loc, S_ISLNK( stats.st.st_mode ) ) );

	// add spaces for utf strings
	int utf_spaces = get_extra_spaces( loc );

	if( !( flags & OPT_L ) ) {
		if( S_ISDIR( stats.st.st_mode ) ) {
			display_padded( max_file_len + utf_spaces, "{b}%s %s/{0}", icon, loc );
			return SUCCESS;
		}

		if( S_ISLNK( stats.st.st_mode ) ) {
			if( stats.lnk_is_dead ) display_padded( max_file_len + utf_spaces, "{r}%s %s{0}", icon, loc );
			else display_padded( max_file_len + utf_spaces, "{y}%s %s{0}", icon, loc );
			return SUCCESS;
		}

		display_padded( max_file_len + utf_spaces, "{g}%s %s{0}", icon, loc );
		return SUCCESS;
	}

	// Now starts the ls with -l option

	display( " " );

	// inode number
	if( flags & OPT_I ) display_padded( 12, "%llu", stats.st.st_ino );

	// directory/link/file
	if( S_ISDIR( stats.st.st_mode ) ) display( "{b}d" );
	else if( S_ISLNK( stats.st.st_mode ) ) display( "{b}l" );
	else display( "{0}-" );

	// permissions
	display( ( stats.st.st_mode & S_IRUSR ) ? "{m}r" : "{0}-" );
	display( ( stats.st.st_mode & S_IWUSR ) ? "{c}w" : "{0}-" );
	display( ( stats.st.st_mode & S_IXUSR ) ? "{r}x" : "{0}-" );
	display( ( stats.st.st_mode & S_IRGRP ) ? "{m}r" : "{0}-" );
	display( ( stats.st.st_mode & S_IWGRP ) ? "{c}w" : "{0}-" );
	display( ( stats.st.st_mode & S_IXGRP ) ? "{r}x" : "{0}-" );
	display( ( stats.st.st_mode & S_IROTH ) ? "{m}r" : "{0}-" );
	display( ( stats.st.st_mode & S_IWOTH ) ? "{c}w" : "{0}-" );
	display( ( stats.st.st_mode & S_IXOTH ) ? "{r}x" : "{0}-" );

	// number of links
	if( flags & OPT_I ) display( "\t" );
	display( "\t{0}%d\t", stats.st.st_nlink );

	// user and group
	struct passwd * usr = getpwuid( stats.st.st_uid );
	struct group * grp = getgrgid( stats.st.st_gid );

	if( usr == NULL ) display_padded( 14, "{g}%d", stats.st.st_uid );
	else display_padded( 15, "{g}%s", usr->pw_name );

	if( grp == NULL ) display( "\t{g}%d", stats.st.st_gid );
	else display( "\t{g}%s", grp->gr_name );

	// file size
	if( flags & OPT_H ) {
		char size[ 30 ];
		format_file_size( stats.st.st_size, size );
		display( "\t{y}%s", size );
	}
	else {
		display( "\t{y}%lluB", stats.st.st_size );
	}

	// last modified time
	char mtime[ 30 ];
	strftime( mtime, 30, "%h %e %H:%M", gmtime( & stats.st.st_mtime ) );
	display( "\t{w}%s", mtime );

	// file/folder name
	if( S_ISDIR( stats.st.st_mode ) ) display( "\t{b}%s %s", icon, loc );
	else if( S_ISLNK( stats.st.st_mode ) ) display( "\t{y}%s %s{0}" , icon, loc );
	else display( "\t{g}%s %s", icon, loc );

	// link info for links
	if( S_ISLNK( stats.st.st_mode ) ) {
		display( " {m}-> {c}%s{0}", stats.lnk_loc );
		if( stats.lnk_is_dead ) display( " {y}[{r}dead link{y}]{0}" );
	}

	display( "{0}\n" );
	return SUCCESS;
}

static int get_stats( const char * path, struct stat_info * stats )
{
	struct stat tmp_st;
	int temp_res = lstat( path, & tmp_st );
	if( stats->lnk_jumps == 0 ) memcpy( & stats->st, & tmp_st, sizeof( struct stat ) );

	if( stats->lnk_jumps == 0 && temp_res != 0 ) {
		return errno;
	}

	if( stats->lnk_jumps == 1 && temp_res != 0 ) {
		stats->lnk_is_dead = true;
		// set the link stats to be same as the file's stats
		memcpy( & stats->lnk_st, & stats->st, sizeof( struct stat ) );
		return SUCCESS;
	}

	if( S_ISLNK( tmp_st.st_mode ) ) {
		stats->lnk_jumps += 1;

		char buf[ 2048 ];
		ssize_t len;
		strcpy( buf, "\0" );
		if( ( len = readlink( path, buf, sizeof( buf ) - 1 ) ) != -1 ) {
			buf[ len ] = '\0';
			if( stats->lnk_jumps == 1 ) strcpy( stats->lnk_loc, buf );
			return get_stats( buf, stats );
		}

		return LOC_NOT_OPENED;
	}

	memcpy( & stats->lnk_st, & tmp_st, sizeof( struct stat ) );
	return SUCCESS;
}

static void format_file_size( const size_t size, char * res )
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
			if( di->d_type == DT_DIR && !( flags & OPT_D ) ) continue;
			if( di->d_type != DT_DIR && !( flags & OPT_F ) ) continue;
		}
		if( strncmp( di->d_name, ".", 1 ) == 0 || strncmp( di->d_name, "..", 2 ) == 0 ) {
			if( ( strcmp( di->d_name, "." ) == 0 || strcmp( di->d_name, ".." ) == 0 ) && !( flags & OPT_A ) ) continue;
			else if( !( flags & OPT_A ) && !( flags & OPT_CAPS_A ) ) continue;
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

static int get_max_file_len( struct str_vec * locs )
{
	int max_len = 0;
	for( int i = 0; i < ( int )locs->count; ++i ) {
		int len = utf8_strlen( str_vec_get( locs, i ) );
		if( max_len < len ) max_len = len;
	}

	return max_len;
}
