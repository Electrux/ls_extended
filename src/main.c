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
#include <unistd.h>
#include <sys/ioctl.h>

#include "../include/core.h"
#include "../include/strvec.h"
#include "../include/cmdopts.h"
#include "../include/help.h"
#include "../include/ls.h"

int main( int argc, char ** argv )
{
	if( argc > 1 && strcmp( argv[ 1 ], "--help" ) == 0 ) {
		display_help( argv[ 0 ] );
		return 0;
	}

	if( argc > 1 && strcmp( argv[ 1 ], "--version" ) == 0 ) {
		display( "{p}Version{0}: {s}%zu{0}.{s}%zu{0}.{s}%zu{0}\n", VERSION_MAIN, VERSION_SUB, VERSION_PATCH );
		return 0;
	}

	struct str_vec * locs = str_vec_create();
	size_t flags = get_cmd_opts( argc, ( const char ** )argv, locs );

	struct winsize ws;
	ioctl( STDOUT_FILENO, TIOCGWINSZ, & ws );

	size_t loc_count = str_vec_get_count( locs );
	int res = 0;
	for( size_t i = 0; i < loc_count; ++i ) {
		res = ls( & ws, str_vec_get( locs, i ), flags, loc_count );
		if( res != 0 ) break;
	}

	str_vec_delete( & locs );
	return res;
}
