/*
	Copyright (c) 2019, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.
	Please read that file and understand the license terms
	before using or altering the project.
*/

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "color.h"
#include "core.h"

void split_file( const char * file, char * name, char * ext )
{
	int ctr = strlen( name );
	int encountered_dot = -1;
	int file_len = strlen( file );
	if( file_len <= 0 ) return;

	strcpy( ext, "\0" );

	if( file[ 0 ] != '.' ) {
		for( int i = file_len; i >= 0; --i ) {
			if( file[ i ] == '.' ) {
				encountered_dot = i;
				break;
			}
		}
	}

	if( encountered_dot >= 0 ) {
		ctr = 0;
		for( int i = encountered_dot; i <= file_len; ++i )
			ext[ ctr++ ] = file[ i ];
		ext[ ctr ] = '\0';
	}

	strcpy( name, file );
}

void disp( FILE * out, const char * fmt, ... )
{
	int fmtlen = strlen( fmt );
	char * res_str = ( char * )malloc( sizeof( char ) * MAX_STR_LEN );
	size_t res_ctr = 0;

	for( int i = 0; i < fmtlen; ++i ) {
		if( fmt[ i ] == '{' ) {
			if( i > 0 && fmt[ i - 1 ] == '\\' ) {
				--res_ctr;
				res_str[ res_ctr++ ] = fmt[ i ];
				continue;
			}
			++i;
			char tmp[ 20 ];
			size_t tctr = 0;
			while( fmt[ i ] != '\0' && fmt[ i ] != '}' ) {
				tmp[ tctr++ ] = fmt[ i ];
				++i;
			}
			if( tctr == 0 ) continue;
			tmp[ tctr ] = '\0';
			const char * col = get_col( tmp );
			if( col == NULL ) continue;
			res_str[ res_ctr ] = '\0';
			strcat( res_str, col );
			res_ctr = strlen( res_str );
			continue;
		}
		res_str[ res_ctr++ ] = fmt[ i ];
	}
	res_str[ res_ctr ] = '\0';

	if( out == NULL ) return;
	va_list args;
	va_start( args, fmt );

	vfprintf( out, res_str, args );

	va_end( args );
	free( res_str );
}

int numlen( unsigned long long num )
{
	int len = !num;
	while( num ) { ++len; num /= 10; }
	return len;
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

	// Latin-1 Supplement are treated to be full width for some reason even
	// though they use half width only:
	// 00A0 - 00FF
	if( val >= 0xC2A0 && val <= 0xC3BF ) return 1;
	// Same for Greek and Coptic set:
	// 0370 - 03FF
	else if( val >= 0xCDB0 && val <= 0xCFBF ) return 1;
	// 3099 - 309C are exceptions to the full width characters in Hiragana
	// These seem like they don't take any space for themselves and use the cells
	// of the previous characters
	else if( val >= 0xE38299 && val <= 0xE3829C ) return 3;
	// CJK Radicals Supplement, Kangxi Radicals, Ideographic Description Characters, CJK Symbols and Punctuation, Hiragana, Katakana,
	// Bopomofo, Hangul Compatibility Jamo, Kanbun, Bopomofo Extended, Katakana Phonetic Extensions, Enclosed CJK Letters and Months,
	// CJK Compatibility, CJK Unified Ideogprahs Extension A, Yijing Hexagram Symbols, CJK Unified Ideographs, Yi Syllables, Yi Radicals:
	// 2E80 - A4CF
	else if( val >= 0xE2BA80 && val <= 0xEA938F ) return 1;
	// Hangul: AC00 - D7AF
	else if( val >= 0xEAB080 && val <= 0xED9EAF ) return 1;
	// High Surrogates, High Private Use Surrogates, Low Surrogates, Private Use Area,
	// CJK Compatibility Ideographs, Alphabetic Presentation Forms:
	// D800 - FDFF
	else if( val >= 0xEDA080 && val <= 0xEFB7BF ) return 1;
	// CJK Compatibility, Small form variants:
	// FE30 - FE6F
	else if( val >= 0xEFB8B0 && val <= 0xEFB9AF ) return 1;

	// Half and / or full width characters:
	// FF00 - FFEF
	if( val >= 0xEFBC80 && val <= 0xEFBFAF ) {
		// Halfwidth: FF65 - FF9F and FFA0 - FFDC
		if( ( val >= 0xEFBDA5 && val <= 0xEFBE9F ) || ( val >= 0xEFBEA0 && val <= 0xEFBF9C ) ) return 2;
		// Full width
		return 1;
	}

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
