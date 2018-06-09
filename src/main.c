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
#include "../include/cmdopts.h"
#include "../include/help.h"
#include "../include/ls.h"

int main( int argc, char ** argv )
{
	if( argc > 1 && strcmp( argv[ 1 ], "help" ) == 0 ) {
		display_help( argv[ 0 ] );
		return 0;
	}

	if( argc > 1 && strcmp( argv[ 1 ], "version" ) == 0 ) {
		display( "{p}Version{0}: {s}%lu{0}.{s}%lu{0}.{s}%lu{0}\n", VERSION_MAIN, VERSION_SUB, VERSION_PATCH );
		return 0;
	}

	char location[ 1024 ];
	size_t flags = get_cmd_opts( argc, ( const char ** )argv, location );

	struct winsize ws;
	ioctl( STDOUT_FILENO, TIOCGWINSZ, & ws );

	return ls( & ws, location, flags );
}
