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
	display( "{p}Usage{0}: {s}%s {0}[{t}OPTIONS{0}] {t}LOCATION{0}\n\n", prog_name );

	display( "{p}Options:\n\n" );

	display( "\t{0}-{s}a {0} - {t}Shows all files{0}/{t}folders{0}, {t}including the hidden ones{0}\n" );
	display( "\t{0}-{s}A {0} - {t}Shows all files{0}/{t}folders{0}, {t}including the hidden{0}, {t}but excluding the {0}. {t}and {0}.. {t}ones{0}\n");
	display( "\t{0}-{s}d {0} - {t}Shows only directories{0}\n" );
	display( "\t{0}-{s}f {0} - {t}Shows only files{0}\n" );
	// TODO:
	//display( "\t{r}Under construction{0}: -{s}g {0} - {t}Shows files{0}/{t}folders with additional github status{0}, {e}can only be used with the {0}-{s}l {e}option{0}\n" );
	display( "\t{0}-{s}h {0} - {t}Shows all sizes in human readable format{0}, {e}can only be used with the {0}-{s}l {e}option{0}\n" );
	display( "\t{0}-{s}i {0} - {t}Shows inode numbers of all files{0}/{t}folders{0}, {e}can only be used with the {0}-{s}l {e}option{0}\n" );
	display( "\t{0}-{s}l {0} - {t}Shows all files{0}/{t}directories in a list form with various information about them{0}\n" );

	display( "\t{0}-{s}s {0} - {t}Sorts the output by directories first{0}\n" );
}
