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

#define OPT_SPACES 8

void display_help( const char * prog_name )
{
	display( "{p}Usage{0}: {s}%s {0}[{t}OPTIONS{0}] {t}LOCATION{0}\n\n", prog_name );

	display( "{p}Options:\n\n" );

	display( "\t{0}--{s}%*s{0} - {t}Show this message and exit\n", OPT_SPACES - 1, "help" );
	display( "\t{0}--{s}%*s{0} - {t}Display version information and exit\n", OPT_SPACES - 1, "version" );

	display( "\n" );

	display( "\t{0}-{s}%*c{0} - {t}Shows all files{0}/{t}folders{0}, {t}including the hidden ones{0}\n", OPT_SPACES, 'a' );
	display( "\t{0}-{s}%*c{0} - {t}Shows all files{0}/{t}folders{0}, {t}including the hidden{0}, {t}but excluding the {0}. {t}and {0}.. {t}ones{0}\n", OPT_SPACES, 'A' );
	display( "\t{0}-{s}%*c{0} - {t}Shows only directories{0}\n", OPT_SPACES, 'd' );
	display( "\t{0}-{s}%*c{0} - {t}Shows only files{0}\n", OPT_SPACES, 'f' );
	// TODO:
	//display( "\t{r}Under construction{0}: -{s}g {0} - {t}Shows files{0}/{t}folders with additional github status{0}, {e}can only be used with the {0}-{s}l {e}option{0}\n" );
	display( "\t{0}-{s}%*c{0} - {t}Shows all sizes in human readable format{0}, {e}can only be used with the {0}-{s}l {e}option{0}\n", OPT_SPACES, 'h' );
	display( "\t{0}-{s}%*c{0} - {t}Shows inode numbers of all files{0}/{t}folders{0}, {e}can only be used with the {0}-{s}l {e}option{0}\n", OPT_SPACES, 'i' );
	display( "\t{0}-{s}%*c{0} - {t}Shows all files{0}/{t}directories in a list form with various information about them{0}\n", OPT_SPACES, 'l' );

	display( "\t{0}-{s}%*c{0} - {t}Sorts the output by directories first{0}\n", OPT_SPACES, 's' );
	display( "\t{0}-{s}%*c{0} - {t}{0}({t}The numeric digit ``one''{0}.) {t}Force output to be one entry per line{0}\n", OPT_SPACES, '1' );
}
