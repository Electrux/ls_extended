/*
	Copyright (c) 2019, Electrux
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

#include "cmdopts.h"
#include "sorts.h"
#include "icons.h"
#include "disp_basic.h"
#include "disp_long.h"
#include "ls.h"

#define MAX_LINK_JUMP_COUNT 32

static vec_t * generate_file_vec( DIR * dir, max_lens_t * maxlens, const char * base_loc, const size_t flags );
static int get_stats( const char * path, stat_info_t * stats );
static void reset_stats( stat_info_t * stats );
static void update_data_and_max_lens( stat_info_t * stats, max_lens_t * maxlens, const size_t flags );
static void format_file_size( const size_t size, char * res );

int ls( const struct winsize * ws, const char * loc, size_t flags, int loc_count )
{
	char final_loc[ MAX_STR_LEN ];
	if( strlen( loc ) + 1 > MAX_STR_LEN ) {
		disp( stderr, "error: too long location string length (max %d charactersd)", MAX_STR_LEN - 1 );
		return EXIT_FAILURE;
	}
	strcpy( final_loc, loc );

	// remove -l dependent flags if -l is not present
	if( !( flags & OPT_L ) ) {
		flags &= ~OPT_G;
		flags &= ~OPT_H;
		flags &= ~OPT_I;
	}

	struct stat tmp_st;
	int tmp_st_res = stat( final_loc, & tmp_st );
	if( tmp_st_res != 0 ) {
		disp( stdout, "{p}Could not fetch information of {r}%s{0} ({p}%s{0})\n", final_loc, strerror( errno ) );
		return errno;
	}
	if( !S_ISDIR( tmp_st.st_mode ) ) {
		stat_info_t stats;
		reset_stats( & stats );

		int err = get_stats( final_loc, & stats );
		if( err != 0 ) return err;
		split_file( final_loc, stats.name, stats.ext );
		if( !( flags & OPT_N ) ) {
			if( S_ISDIR( stats.lnk_st.st_mode ) ) {
				strcpy( stats.icon, get_dir_icon( final_loc, S_ISLNK( stats.st.st_mode ) ) );
			} else {
				strcpy( stats.icon, get_file_icon( stats.name, stats.ext, S_ISLNK( stats.st.st_mode ) ) );
			}
			strcat( stats.icon, " " );
			stats.iconlen = utf8_strlen( stats.icon );
		}
		if( !( flags & OPT_L ) ) {
			int spcs = 2;
			if( S_ISLNK( stats.st.st_mode ) ) {
				strcat( stats.name, "@" );
				--spcs;
			}
			if( S_ISDIR( stats.lnk_st.st_mode ) ) {
				strcat( stats.name, "/" );
				--spcs;
			}
			if( spcs == 2 ) strcat( stats.name, "  " );
			else if( spcs == 1 ) strcat( stats.name, " " );
		}
		update_data_and_max_lens( & stats, NULL, flags );
		vec_t * loc = vec_create( sizeof( stat_info_t ), NULL );
		vec_add( loc, & stats );
		if( !( flags & OPT_L ) ) disp_basic( loc, ws, NULL, flags );
		else disp_long( loc, ws, NULL, flags );
		vec_destroy( & loc );
		return 0;
	}

	if( final_loc[ strlen( final_loc ) - 1 ] != '/' ) {
		strcat( final_loc, "/" );
	}

	char currdir[ MAX_STR_LEN ];
	getcwd( currdir, MAX_STR_LEN - 1 );
	int cd_res = chdir( final_loc );
	if( cd_res != 0 ) {
		disp( stderr, "{p}Unable to open directory{0}: {r}%s{0} ({p}%s{0})\n", final_loc, strerror( errno ) );
		return LOC_NOT_OPENED;
	}

	DIR * dir = opendir( "." );
	if( dir == NULL ) {
		disp( stderr, "{p}Unable to open directory{0}: {r}%s{0} ({p}%s{0})\n", final_loc, strerror( errno ) );
		return LOC_NOT_OPENED;
	}

	max_lens_t maxlens;
	vec_t * locs = generate_file_vec( dir, & maxlens, "./", flags );
	closedir( dir );
	chdir( currdir );

	if( locs == NULL ) {
		return VEC_INIT_FAIL;
	}

	if( !( flags & OPT_L ) ) {
		disp_basic( locs, ws, & maxlens, flags );
		goto end;
	}
	disp_long( locs, ws, & maxlens, flags );
end:
	vec_destroy( & locs );
	return 0;
}


static vec_t * generate_file_vec( DIR * dir, max_lens_t * maxlens, const char * base_loc, const size_t flags )
{
	struct dirent * di = NULL;
	vec_t * locs = vec_create( sizeof( stat_info_t ), NULL );
	vec_t * dir_locs = NULL;
	if( flags & OPT_S ) dir_locs = vec_create( sizeof( stat_info_t ), NULL );

	maxlens->name = 0;
	maxlens->user = 0;
	maxlens->grp = 0;
	maxlens->size = 0;
	maxlens->inode = 0;
	maxlens->links = 0;

	if( locs == NULL ) {
		disp( stderr, "{p}Unable to create vector {r}locs{0}\n" );
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

		stat_info_t stats;
		reset_stats( & stats );
		char loc[ MAX_STR_LEN ];
		strcpy( loc, base_loc );
		strcat( loc, di->d_name );

		int err = get_stats( loc, & stats );
		if( err != 0 ) goto fail;

		split_file( di->d_name, stats.name, stats.ext );
		if( !( flags & OPT_N ) ) {
			if( S_ISDIR( stats.lnk_st.st_mode ) ) {
				strcpy( stats.icon, get_dir_icon( di->d_name, S_ISLNK( stats.st.st_mode ) ) );
			} else {
				strcpy( stats.icon, get_file_icon( stats.name, stats.ext, S_ISLNK( stats.st.st_mode ) ) );
			}
			strcat( stats.icon, " " );
			stats.iconlen = utf8_strlen( stats.icon );
		}

		if( !( flags & OPT_L ) ) {
			int spcs = 2;
			if( S_ISLNK( stats.st.st_mode ) ) {
				strcat( stats.name, "@" );
				--spcs;
			}
			if( S_ISDIR( stats.lnk_st.st_mode ) ) {
				strcat( stats.name, "/" );
				--spcs;
			}
			if( spcs == 2 ) strcat( stats.name, "  " );
			else if( spcs == 1 ) strcat( stats.name, " " );
		}

		stats.namelen = utf8_strlen( stats.name );
		if( maxlens->name < stats.namelen + stats.iconlen ) maxlens->name = stats.namelen + stats.iconlen;
		stats.width = get_extra_spaces( stats.name );

		update_data_and_max_lens( & stats, maxlens, flags );
		if( ( flags & OPT_S ) && S_ISDIR( stats.lnk_st.st_mode ) ) vec_add( dir_locs, & stats );
		else vec_add( locs, & stats );
	}

	if( flags & OPT_T ) {
		vec_sort( locs, sort_mtime );
	} else if( flags & OPT_CAPS_X ) {
		vec_sort( locs, sort_ext );
	} else if( flags & OPT_CAPS_S ) {
		vec_sort( locs, sort_size );
	} else {
		vec_sort( locs, sort_name );
	}

	if( flags & OPT_S ) {
		if( flags & OPT_T ) {
			vec_sort( dir_locs, sort_mtime );
		} else if( flags & OPT_CAPS_X ) {
			vec_sort( dir_locs, sort_ext );
		} else if( flags & OPT_CAPS_S ) {
			vec_sort( dir_locs, sort_size );
		} else {
			vec_sort( dir_locs, sort_name );
		}
		for( int i = 0; i < vec_count( locs ); ++i )
			vec_add( dir_locs, vec_get_data( locs, i ) );
		vec_destroy( & locs );
		locs = dir_locs;
		dir_locs = NULL;
	}

	return locs;
fail:
	vec_destroy( & locs );
	if( flags & OPT_S && dir_locs != NULL ) vec_destroy( & dir_locs );
	return NULL;
}

static int get_stats( const char * path, stat_info_t * stats )
{
	struct stat tmp_st;
	int temp_res = lstat( path, & tmp_st );
	if( stats->lnk_jumps == 0 ) memcpy( & stats->st, & tmp_st, sizeof( struct stat ) );

	if( stats->lnk_jumps == 0 && temp_res != 0 ) {
		fprintf( stderr, "error: errno %d for path: %s\n", errno, path );
		return errno;
	}

	if( stats->lnk_jumps >= 1 && temp_res != 0 ) {
		stats->lnk_is_dead = true;
		// set the link stats to be same as the file's stats
		memcpy( & stats->lnk_st, & stats->st, sizeof( struct stat ) );
		return SUCCESS;
	}

	if( S_ISLNK( tmp_st.st_mode ) ) {
		stats->lnk_jumps += 1;

		if( stats->lnk_jumps >= MAX_LINK_JUMP_COUNT ) {
			return SUCCESS;
		}

		char buf[ 2048 ];
		ssize_t len;
		strcpy( buf, "\0" );
		if( ( len = readlink( path, buf, sizeof( buf ) - 1 ) ) != -1 ) {
			buf[ len ] = '\0';
			if( stats->lnk_jumps == 1 ) strcpy( stats->lnk_loc, buf );
			int res = get_stats( buf, stats );
			return res;
		}

		return LOC_NOT_OPENED;
	}

	memcpy( & stats->lnk_st, & tmp_st, sizeof( struct stat ) );
	return SUCCESS;
}

static void reset_stats( stat_info_t * stats )
{
	stats->lnk_jumps = 0;
	stats->lnk_is_dead = false;
	stats->name[ 0 ] = '\0';
	stats->ext[ 0 ] = '\0';
	stats->icon[ 0 ] = '\0';
	stats->lnk_loc[ 0 ] = '\0';
	stats->size[ 0 ] = '\0';
	stats->user[ 0 ] = '\0';
	stats->grp[ 0 ] = '\0';
	stats->width = 0;
	stats->namelen = 0;
	stats->iconlen = 0;
}

static void update_data_and_max_lens( stat_info_t * stats, max_lens_t * maxlens, const size_t flags )
{
	int len;
	if( !( flags & OPT_L ) ) {
		return;
	}

	if( maxlens != NULL ) {
		len = numlen( stats->st.st_nlink );
		if( maxlens->links < len ) maxlens->links = len;
	}

	struct passwd * usr = getpwuid( stats->st.st_uid );
	struct group * grp = getgrgid( stats->st.st_gid );

	if( usr == NULL ) {
		sprintf( stats->user, "%zu", ( size_t )stats->st.st_uid );
		if( maxlens != NULL ) len = numlen( stats->st.st_uid );
	} else {
		strcpy( stats->user, usr->pw_name );
		if( maxlens != NULL ) len = utf8_strlen( usr->pw_name );
	}
	if( maxlens != NULL && maxlens->user < len ) maxlens->user = len;

	if( grp == NULL ) {
		sprintf( stats->grp, "%zu", ( size_t )stats->st.st_gid );
		if( maxlens != NULL ) len = numlen( stats->st.st_gid );
	} else {
		strcpy( stats->grp, grp->gr_name );
		if( maxlens != NULL ) len = utf8_strlen( grp->gr_name );
	}
	if( maxlens != NULL && maxlens->grp < len ) maxlens->grp = len;

	if( maxlens != NULL && flags & OPT_I ) {
		len = numlen( stats->st.st_ino );
		if( maxlens->inode < len ) maxlens->inode = len;
	}

	if( flags & OPT_H ) {
		format_file_size( stats->st.st_size, stats->size );
		if( maxlens != NULL ) len = utf8_strlen( stats->size );
	} else {
		sprintf( stats->size, "%zu", ( size_t )stats->st.st_size );
		if( maxlens != NULL ) len = numlen( stats->st.st_size );
	}
	if( maxlens != NULL && maxlens->size < len ) maxlens->size = len;
	return;
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
	while( size_res > 1024 && div_count < 4 ) {
		size_res /= 1024;
		++div_count;
	}

	snprintf( res, 30, "%.1f", size_res );

	if( div_count == 1 ) strcat( res, "K" );
	else if( div_count == 2 ) strcat( res, "M" );
	else if( div_count == 3 ) strcat( res, "G" );
	else if( div_count == 4 ) strcat( res, "T" );
}
