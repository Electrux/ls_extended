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
#include <stdint.h>
#include <inttypes.h>
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

void _display( const int global_padding, const int option, const char * fmt, ... )
{
	char * res_str = ( char * )malloc( sizeof( char ) * MAX_STR_MEM_ALLOC );
	size_t res_ctr = 0;

	va_list args;
	va_start( args, fmt );

	int padding = 0, pad_start = -1;

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
			while( * fmt != '\0' && * fmt != '}' ) {
				tmp[ tmp_ctr++ ] = * fmt++;
			}
			if( * fmt == '\0' ) continue;
			++fmt;

			if( tmp_ctr >= 10 ) tmp_ctr = 9;
			tmp[ tmp_ctr ] = '\0';
			const char * tmp_res = get_color( tmp );
			// invalid color
			if( strcmp( tmp_res, "0" ) == 0 ) continue;

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

			if( * fmt == '*' ) {
				int i = va_arg( args, int );
				padding = i;
				pad_start = res_ctr;
				++fmt;
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
				snprintf( f_str, sizeof( f_str ), "%.2f", f );
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
				snprintf( st_str, sizeof( st_str ), "%" PRIu64, llu );
				res_str[ res_ctr ] = '\0';
				strcat( res_str, st_str );
				res_ctr = strlen( res_str );
				++fmt;
				++fmt;
			}

			if( padding ) {
				// If there was no other format string after *, don't increment fmt;
				if( res_ctr - pad_start != 0 ) ++fmt;

				int pad_count = padding - res_ctr + pad_start;
				for( int i = 0; i < pad_count; ++i ) {
					res_str[ res_ctr++ ] = ' ';
				}
				padding = 0;
				pad_start = -1;
				continue;
			}

			++fmt;
			continue;
		}

		res_str[ res_ctr++ ] = * fmt++;
	}

	va_end( args );

	res_str[ res_ctr ] = '\0';

	if( global_padding ) {
		fprintf( stdout, "%-*s", global_padding, res_str );
		free( res_str );
		return;
	}

	// the second case is of test in which case, nothing is to be printed
	if( option == 1 ) fprintf( stderr, "%s", res_str );
	else if( option != 2 ) fprintf( stdout, "%s", res_str );

	free( res_str );
}

uint8_t extra_space_count( const char * str, const int used_bytes )
{
	// 1 shift for chinese characters instead of 2 because they
	// already take space of 2 english characters and are of 3 bytes each
	long long val = 0;
	for( int i = 0; i < used_bytes; ++i ) {
		val <<= 8;
		val |= (int)( unsigned char )str[ i ];
	}

	// Hex representations
	if( val >= 0xE4B880 && val <= 0xE9BFBF ) return 1;
	else if( val >= 0xE39080   && val <= 0xE4B6BF   ) return 1;
	else if( val >= 0xEFA480   && val <= 0xEFABBF   ) return 1;
	else if( val >= 0xF0A08080 && val <= 0xF0AA9B9F ) return 1;
	else if( val >= 0xF0AA9C80 && val <= 0xF0AB9CBF ) return 1;
	else if( val >= 0xF0AB9D80 && val <= 0xF0ABA09F ) return 1;
	else if( val >= 0xF0ABA0A0 && val <= 0xF0ACBAAF ) return 1;
	else if( val >= 0xF0AFA080 && val <= 0xF0AFA89F ) return 1;

	return 2;
}

uint64_t get_extra_spaces( const char * str )
{
	int extra_spaces = 0;
	int move_ahead;

	while( * str ) {
		uint8_t c = ( unsigned char )( * str );
		if( c >= 0 && c <= 127 ) move_ahead = 1;
		else if( ( c & 0xE0 ) == 0xC0 ) move_ahead = 2;
		else if( ( c & 0xF0 ) == 0xE0 ) move_ahead = 3;
		else if( ( c & 0xF8 ) == 0xF0 ) move_ahead = 4;

		if( move_ahead > 1 ) extra_spaces += extra_space_count( str, move_ahead );

		while( move_ahead-- ) str++;
	}
	return extra_spaces;
}

// The code below is courtesy of:
//   http://www.daemonology.net/blog/2008-06-05-faster-utf8-strlen.html
//

#define ONEMASK ( ( size_t )( -1 ) / 0xFF )

size_t utf8_strlen( const char * _s )
{
	const char * s;
	size_t count = 0;
	size_t u;
	unsigned char b;

	/* Handle any initial misaligned bytes. */
	for( s = _s; ( uintptr_t )( s ) & ( sizeof( size_t ) - 1 ); s++ ) {
		b = * s;
		/* Exit if we hit a zero byte. */
		if( b == '\0' ) goto done;
		/* Is this byte NOT the first byte of a character? */
		count += ( b >> 7 ) & ( ( ~b ) >> 6 );
	}

	/* Handle complete blocks. */
	for( ; ; s += sizeof( size_t ) ) {
		/* Prefetch 256 bytes ahead. */
		__builtin_prefetch( & s[ 256 ], 0, 0 );
		/* Grab 4 or 8 bytes of UTF-8 data. */
		u = * ( size_t * )( s );
		/* Exit the loop if there are any zero bytes. */
		if( ( u - ONEMASK ) & ( ~u ) & ( ONEMASK * 0x80 ) ) break;
		/* Count bytes which are NOT the first byte of a character. */
		u = ( ( u & ( ONEMASK * 0x80 ) ) >> 7 ) & ( ( ~u ) >> 6 );
		count += ( u * ONEMASK ) >> ( ( sizeof( size_t ) - 1 ) * 8 );
	}

	/* Take care of any left-over bytes. */
	for( ; ; s++ ) {
		b = * s;
		/* Exit if we hit a zero byte. */
		if ( b == '\0' ) break;
		/* Is this byte NOT the first byte of a character? */
		count += ( b >> 7 ) & ( ( ~b ) >> 6 );
	}

done:
	return ( ( s - _s ) - count );
}
