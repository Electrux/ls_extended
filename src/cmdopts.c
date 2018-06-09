/*
	Copyright (c) 2018, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#include <string.h>

#include "../include/cmdopts.h"

size_t get_cmd_opts( const int argc, const char ** argv, char * location )
{
	size_t flags = 0;
	uint8_t found_loc = 0;

	for( int i = 1; i < argc; ++i ) {
		// fetch the location to be ls'ed in
		if( argv[ i ][ 0 ] != '-' ) {
			strcpy( location, argv[ i ] );
			found_loc = 1;
			continue;
		}

		size_t len = strlen( argv[ i ] );
		for( size_t j = 1; j <= len; ++j ) {
			if( argv[ i ][ j ] == 'a' ) flags |= OPT_A;
			else if( argv[ i ][ j ] == 'd' ) flags |= OPT_D;
			else if( argv[ i ][ j ] == 'f' ) flags |= OPT_F;
			else if( argv[ i ][ j ] == 'g' ) flags |= OPT_G;
			else if( argv[ i ][ j ] == 'h' ) flags |= OPT_H;
			else if( argv[ i ][ j ] == 'i' ) flags |= OPT_I;
			else if( argv[ i ][ j ] == 'l' ) flags |= OPT_L;
			else if( argv[ i ][ j ] == 't' ) flags |= OPT_T;
			else if( argv[ i ][ j ] == 's' ) flags |= OPT_S;
		}
	}

	if( !found_loc ) strcpy( location, "\0" );
	return flags;
}
