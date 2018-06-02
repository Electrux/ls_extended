/*
	Copyright (c) 2018, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#include <stdio.h>

#include "../include/core.h"

#include "../include/help.h"

void display_help( const char * prog_name )
{
	printf( "{p}Usage{0}: {s}%s {0}[{t}OPTIONS{0}] {t}LOCATION{0}\n\n", prog_name );
}
