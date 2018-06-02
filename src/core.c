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
#include <stdarg.h>
#include <strings.h>

#include "../include/core.h"

static const char * get_color( char * id );

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

void _display( const int option, const char * fmt, ... )
{
	char * res_str = ( char * )malloc( sizeof( char ) * strlen( fmt ) * 3 );
	size_t res_ctr = 0;

	va_list args;
	va_start( args, fmt );

	while( * fmt != '\0' ) {
		if( * fmt == '{' && *( fmt + 1 ) != '\0' ) {
			++fmt;
			if( * fmt == '{' ) {
				res_str[ res_ctr++ ] = * fmt;
				++fmt;
				continue;
			}

			char tmp[ 10 ];
			uint8_t tmp_ctr = 0;
			while( * fmt != '\0' && * fmt != '}') {
				tmp[ tmp_ctr++ ] = * fmt++;
			}
			if( * fmt == '\0' ) continue;
			++fmt;

			if( tmp_ctr >= 10 ) tmp_ctr = 9;
			tmp[ tmp_ctr ] = '\0';
			const char * tmp_res = get_color( tmp );
			// invalid color
			if( strcmp( tmp_res, "0") == 0 ) continue;

			res_str[ res_ctr ] = '\0';
			strcat( res_str, tmp_res );
			// no + 1 because the return value is length and we use it as index so we need to - 1 it anyway
			res_ctr = strlen( res_str );
			continue;
		}

		if( * fmt == '%' && *( fmt + 1 ) != '\0' ) {
			++fmt;
			if( * fmt == '%' ) {
				res_str[ res_ctr++ ] = * fmt;
				++fmt;
				continue;
			}

			if( * fmt == 'c' ) {
				char c = ( char )va_arg( args, int );
				res_str[ res_ctr++ ] = c;
			}
			if( * fmt == 's' ) {
				const char * str = va_arg( args, const char * );
				res_str[ res_ctr ] = '\0';
				strcat( res_str, str );
				res_ctr = strlen( res_str );
			}
			if( * fmt == 'd' ) {
				int i = va_arg( args, int );
				char i_str[ 20 ];
				snprintf( i_str, sizeof( i_str ), "%d", i );
				res_str[ res_ctr ] = '\0';
				strcat( res_str, i_str );
				res_ctr = strlen( res_str );
			}
			if( * fmt == 'f' ) {
				double f = va_arg( args, double );
				char f_str[ 10 ];
				gcvt( f, sizeof( f_str ), f_str );
				res_str[ res_ctr ] = '\0';
				strcat( res_str, f_str );
				res_ctr = strlen( res_str );
			}
			++fmt;
			continue;
		}

		res_str[ res_ctr++ ] = * fmt++;
	}

	va_end( args );

	res_str[ res_ctr ] = '\0';

	// the second case is of test in which case, nothing is to be printed
	if( option == 1 ) fprintf( stderr, "%s", res_str );
	else if( option != 2 ) fprintf( stdout, "%s", res_str );

	free( res_str );
}

static const char * get_color( char * id )
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
