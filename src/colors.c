/*
	Copyright (c) 2018, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#include <strings.h>

#include "../include/colors.h"

const char * COLORS[ LAST ] = {
	// Reset to no font customization
	"\033[0m",

	// Red, Green, Yellow, Blue, Magenta, Cyan, White colors
	"\033[0;31m",
	"\033[0;32m",
	"\033[0;33m",
	"\033[0;34m",
	"\033[0;35m",
	"\033[0;36m",
	"\033[0;37m",

	// Bold variants
	"\033[1;31m",
	"\033[1;32m",
	"\033[1;33m",
	"\033[1;34m",
	"\033[1;35m",
	"\033[1;36m",
	"\033[1;37m",

	// Primary, Secondary, Tertiary, and Extra colors
	"\033[0;35m",
	"\033[0;33m",
	"\033[0;37m",
	"\033[0;36m",
};

const char * get_color( const char * id )
{
	if( strcasecmp( id, "0" ) == 0 ) return COLORS[ RESET ];

	else if( strcasecmp( id, "r" ) == 0 ) return COLORS[ RED ];
	else if( strcasecmp( id, "g" ) == 0 ) return COLORS[ GREEN ];
	else if( strcasecmp( id, "y" ) == 0 ) return COLORS[ YELLOW ];
	else if( strcasecmp( id, "b" ) == 0 ) return COLORS[ BLUE ];
	else if( strcasecmp( id, "m" ) == 0 ) return COLORS[ MAGENTA ];
	else if( strcasecmp( id, "c" ) == 0 ) return COLORS[ CYAN ];
	else if( strcasecmp( id, "w" ) == 0 ) return COLORS[ WHITE ];

	else if( strcasecmp( id, "br" ) == 0 ) return COLORS[ BOLD_RED ];
	else if( strcasecmp( id, "bg" ) == 0 ) return COLORS[ BOLD_GREEN ];
	else if( strcasecmp( id, "by" ) == 0 ) return COLORS[ BOLD_YELLOW ];
	else if( strcasecmp( id, "bb" ) == 0 ) return COLORS[ BOLD_BLUE ];
	else if( strcasecmp( id, "bm" ) == 0 ) return COLORS[ BOLD_MAGENTA ];
	else if( strcasecmp( id, "bc" ) == 0 ) return COLORS[ BOLD_CYAN ];
	else if( strcasecmp( id, "bw" ) == 0 ) return COLORS[ BOLD_WHITE ];

	else if( strcasecmp( id, "p" ) == 0 ) return COLORS[ PRIMARY ];
	else if( strcasecmp( id, "s" ) == 0 ) return COLORS[ SECONDARY ];
	else if( strcasecmp( id, "t" ) == 0 ) return COLORS[ TERTIARY ];
	else if( strcasecmp( id, "e" ) == 0 ) return COLORS[ EXTRA ];

	return "0";
}
