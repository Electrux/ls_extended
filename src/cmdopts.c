/*
	Copyright (c) 2019, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.
	Please read that file and understand the license terms
	before using or altering the project.
*/

#include <stdint.h>
#include <string.h>

#include "cmdopts.h"

size_t get_cmd_opts( const int argc, const char ** argv, vec_t * locs )
{
	size_t flags = 0;
	uint8_t found_loc = 0;

	for( int i = 1; i < argc; ++i ) {
		// fetch the location to be ls'ed in
		if( argv[ i ][ 0 ] != '-' ) {
			vec_add( locs, argv[ i ] );
			continue;
		}

		size_t len = strlen( argv[ i ] );
		for( size_t j = 1; j <= len; ++j ) {
			if( argv[ i ][ j ] == 'a' ) flags |= OPT_A;
			else if( argv[ i ][ j ] == 'A' ) flags |= OPT_CAPS_A;
			else if( argv[ i ][ j ] == 'd' ) flags |= OPT_D;
			else if( argv[ i ][ j ] == 'f' ) flags |= OPT_F;
			else if( argv[ i ][ j ] == 'g' ) flags |= OPT_G;
			else if( argv[ i ][ j ] == 'h' ) flags |= OPT_H;
			else if( argv[ i ][ j ] == 'i' ) flags |= OPT_I;
			else if( argv[ i ][ j ] == 'l' ) flags |= OPT_L;
			else if( argv[ i ][ j ] == 't' ) flags |= OPT_T;
			else if( argv[ i ][ j ] == 's' ) flags |= OPT_S;
			else if( argv[ i ][ j ] == '1' ) flags |= OPT_1;
		}
	}

	if( vec_count( locs ) == 0 ) vec_add( locs, "." );
	return flags;
}