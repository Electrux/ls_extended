/*
	Copyright (c) 2019, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.
	Please read that file and understand the license terms
	before using or altering the project.
*/

#include "cmdopts.h"
#include "disp_basic.h"

static void set_widths( vec_t * locs, const int rows, const int cols );
static void calc_num_rows_cols( const vec_t * locs, max_lens_t * maxlens, const struct winsize * ws, int * num_rows, int * num_cols, const size_t flags );

void disp_basic( vec_t * locs, const struct winsize * ws, max_lens_t * maxlens, const size_t flags )
{
	if( maxlens == NULL ) {
		const stat_info_t * stats = vec_get_data( locs, 0 );
		if( S_ISDIR( stats->lnk_st.st_mode ) ) {
			disp( stdout, "{b}%s%s{0}", stats->icon, stats->name );
		} else if( S_ISLNK( stats->st.st_mode ) ) {
			if( stats->lnk_is_dead ) disp( stdout, "{r}%s%s{0}", stats->icon, stats->name );
			else disp( stdout, "{y}%s%s{0}", stats->icon, stats->name );
		} else {
			disp( stdout, "{g}%s%s{0}", stats->icon, stats->name );
		}
		disp( stdout, "\n" );
		return;
	}
	int rows, cols;
	calc_num_rows_cols( locs, maxlens, ws, & rows, & cols, flags );
	set_widths( locs, rows, cols );

	int item_line_ctr = 0;
	int loc_count = vec_count( locs );

	int count = vec_count( locs );
	for( int r = 0; r < rows; ++r ) {
		for( int c = 0; c < cols; ++c ) {
			int i = c * rows + r;
			if( i >= count ) continue;
			const stat_info_t * stats = vec_get_data( locs, i );
			if( S_ISDIR( stats->lnk_st.st_mode ) ) {
				disp( stdout, "{b}%s%-*s{0}", stats->icon, c == cols - 1 ? -1 : stats->width, stats->name );
			} else if( S_ISLNK( stats->st.st_mode ) ) {
				if( stats->lnk_is_dead ) disp( stdout, "{r}%s%-*s{0}", stats->icon, c == cols - 1 ? -1 : stats->width, stats->name );
				else disp( stdout, "{y}%s%-*s{0}", stats->icon, c == cols - 1 ? -1 : stats->width, stats->name );
			} else {
				disp( stdout, "{g}%s%-*s{0}", stats->icon, c == cols - 1 ? -1 : stats->width, stats->name );
			}
		}
		disp( stdout, "\n" );
	}
}

static void set_widths( vec_t * locs, const int rows, const int cols )
{
	int count = vec_count( locs );
	int i = 0;
	for( int c = 0; c < cols; ++c ) {
		int maxlen = 0;
		int tmpi = i;
		for( int r = 0; r < rows; ++r ) {
			if( i >= count ) continue;
			const stat_info_t * stats = vec_get_data( locs, i );
			if( maxlen < stats->namelen + 1 ) maxlen = stats->namelen + 1;
			++i;
		}
		i = tmpi;
		for( int r = 0; r < rows; ++r ) {
			if( i >= count ) continue;
			stat_info_t * stats = vec_get_data_mut( locs, i );
			stats->width += maxlen;
			++i;
		}
	}
}

static void calc_num_rows_cols( const vec_t * locs, max_lens_t * maxlens, const struct winsize * ws, int * num_rows, int * num_cols, const size_t flags )
{
	int count = vec_count( locs );
	if( maxlens->name == 0 ) maxlens->name = 1;
	* num_cols = ( ws->ws_col / maxlens->name );
	if( * num_cols <= 0 ) * num_cols = 1;
	* num_rows = count / * num_cols;
	if( * num_rows <= 0 ) * num_rows = 1;
	if( * num_cols > count ) { * num_rows = 1; * num_cols = count; return; }
	if( flags & OPT_1 ) { * num_rows = count; * num_cols = 1; return; }

	while( ( * num_rows ) * ( * num_cols ) < count ) {
		++( * num_cols );
	}
	int tot_len;
	int i;
	int can_reduce_rows = -1;
begin:
	tot_len = 0;
	i = 0;
	for( int c = 0; c < * num_cols; ++c ) {
		int maxlen = 0;
		// no + 1 in maxlen at last column
		int lastadd = c == * num_cols - 1 ? 0 : 1;
		for( int r = 0; r < * num_rows; ++r ) {
			if( i >= count ) continue;
			const stat_info_t * stats = vec_get_data( locs, i );
			if( maxlen < stats->namelen + stats->iconlen + stats->width + lastadd )
				maxlen = stats->namelen + stats->iconlen + stats->width + lastadd;
			++i;
		}
		tot_len += maxlen;
	}
	// one for \n
	tot_len -= 1;
	if( tot_len >= ws->ws_col ) {
		if( can_reduce_rows != -1 ) can_reduce_rows = 0;
		--( * num_cols );
		++( * num_rows );
		goto begin;
	}
	if( can_reduce_rows != 0 && * num_rows > 1 && ( ( ( * num_rows ) - 1 ) * ( ( * num_cols ) + 1 ) >= count ) ) {
		can_reduce_rows = 1;
		++( * num_cols );
		--( * num_rows );
		goto begin;
	}
}
