/*
	Copyright (c) 2019, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.
	Please read that file and understand the license terms
	before using or altering the project.
*/

#include <time.h>

#include "cmdopts.h"
#include "disp_long.h"

static int update_max_lens( const stat_info_t * stats, max_lens_t * maxlens, const size_t flags );

void disp_long( vec_t * locs, const struct winsize * ws, const max_lens_t * maxlens, const size_t flags )
{
	int count = vec_count( locs );
	for( int i = 0; i < count; ++i ) {
		const stat_info_t * stats = vec_get_data( locs, i );

		// inode number
		if( flags & OPT_I ) {
			disp( stdout, "%-*llu ", maxlens->inode, stats->st.st_ino );
		}

		// directory/link/file
		if( S_ISDIR( stats->st.st_mode ) ) disp( stdout, "{b}d" );
		else if( S_ISLNK( stats->st.st_mode ) ) disp( stdout, "{b}l" );
		else disp( stdout, "{0}-" );
		// permissions
		disp( stdout, ( stats->st.st_mode & S_IRUSR ) ? "{m}r" : "{0}-" );
		disp( stdout, ( stats->st.st_mode & S_IWUSR ) ? "{c}w" : "{0}-" );
		disp( stdout, ( stats->st.st_mode & S_IXUSR ) ? "{r}x" : "{0}-" );
		disp( stdout, ( stats->st.st_mode & S_IRGRP ) ? "{m}r" : "{0}-" );
		disp( stdout, ( stats->st.st_mode & S_IWGRP ) ? "{c}w" : "{0}-" );
		disp( stdout, ( stats->st.st_mode & S_IXGRP ) ? "{r}x" : "{0}-" );
		disp( stdout, ( stats->st.st_mode & S_IROTH ) ? "{m}r" : "{0}-" );
		disp( stdout, ( stats->st.st_mode & S_IWOTH ) ? "{c}w" : "{0}-" );
		disp( stdout, ( stats->st.st_mode & S_IXOTH ) ? "{r}x" : "{0}-" );
		disp( stdout, ( stats->st.st_mode & S_ISVTX ) ? "{r}s" : "{0} " );
		disp( stdout, " " );

		// number of links
		disp( stdout, " {0}%-*d", maxlens->links + 1, stats->st.st_nlink );

		// user and group
		disp( stdout, "{g}%-*s", maxlens->user + 1, stats->user );
		disp( stdout, "{g}%-*s", maxlens->grp + 1, stats->grp );

		// file size
		disp( stdout, "{y}%-*s", maxlens->size + 1, stats->size );

		// last modified time
		char mtime[ 30 ];
		strftime( mtime, 30, "%h %e %H:%M", gmtime( & stats->st.st_mtime ) );
		disp( stdout, "{m}%s ", mtime );

		// file/folder name
		if( S_ISDIR( stats->lnk_st.st_mode ) ) {
			disp( stdout, "{b}%s%-*s{0}", stats->icon, stats->width, stats->name );
		} else if( S_ISLNK( stats->st.st_mode ) ) {
			if( stats->lnk_is_dead ) disp( stdout, "{r}%s%-*s{0}", stats->icon, stats->width, stats->name );
			else disp( stdout, "{y}%s%-*s{0}", stats->icon, stats->width, stats->name );
		} else {
			disp( stdout, "{g}%s%-*s{0}", stats->icon, stats->width, stats->name );
		}

		// link info for links
		if( S_ISLNK( stats->st.st_mode ) ) {
			disp( stdout, " {m}-> {c}%s{0}", stats->lnk_loc );
			if( stats->lnk_is_dead ) disp( stdout, " {y}[{r}dead link{y}]{0}" );
		}

		disp( stdout, "{0}\n" );
	}
}
