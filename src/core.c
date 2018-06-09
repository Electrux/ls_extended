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
#include <string.h>
#include <sys/ioctl.h>

#include "../include/colors.h"

#include "../include/core.h"

#define MAX_STR_MEM_ALLOC 2048

void split_file( const char * file, char * name, char * ext )
{
	int ctr = 0;
	int encountered_dot = -1;
	int file_len = strlen( file );

	for( int i = file_len; i >= 0; --i ) {
		if( file[ i ] == '.' ) {
			encountered_dot = i;
			break;
		}
	}

	if( encountered_dot >= 0 ) {
		for( int i = encountered_dot + 1; i <= file_len; ++i )
			ext[ ctr++ ] = file[ i ];
		ext[ ctr ] = '\0';

		ctr = 0;
		for( int i = 0; i < encountered_dot; ++i )
			name[ ctr++ ] = file[ i ];
		name[ ctr ] = '\0';
		return;
	}

	strcpy( name, file );
	ext[ 0 ] = '\0';
}

void _display( const int padding, const int option, const char * fmt, ... )
{
	char * res_str = ( char * )malloc( sizeof( char ) * MAX_STR_MEM_ALLOC );
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
			if( * fmt == 'z' && *( fmt + 1 ) != '\0' && *( fmt + 1 ) == 'u' ) {
				size_t st = va_arg( args, size_t );
				char st_str[ 30 ];
				snprintf( st_str, sizeof( st_str ), "%zu", st );
				res_str[ res_ctr ] = '\0';
				strcat( res_str, st_str );
				res_ctr = strlen( res_str );
				++fmt;
			}
			if( * fmt == 'l' && *( fmt + 1 ) != '\0' && *( fmt + 1 ) == 'l' && *( fmt + 2 ) != '\0' && *( fmt + 2 ) == 'u' ) {
				uint64_t llu = va_arg( args, uint64_t );
				char st_str[ 30 ];
				snprintf( st_str, sizeof( st_str ), "%llu", llu );
				res_str[ res_ctr ] = '\0';
				strcat( res_str, st_str );
				res_ctr = strlen( res_str );
				++fmt;
				++fmt;
			}
			++fmt;
			continue;
		}

		res_str[ res_ctr++ ] = * fmt++;
	}

	va_end( args );

	res_str[ res_ctr ] = '\0';

	if( padding ) {
		fprintf( stdout, "%-*s", padding, res_str );
		free( res_str );
		return;
	}

	// the second case is of test in which case, nothing is to be printed
	if( option == 1 ) fprintf( stderr, "%s", res_str );
	else if( option != 2 ) fprintf( stdout, "%s", res_str );

	free( res_str );
}
