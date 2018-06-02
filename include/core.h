/*
	Copyright (c) 2018, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#ifndef CORE_H
#define CORE_H

#include <stdlib.h>

static const uint8_t VERSION_MAIN = 0;
static const uint8_t VERSION_SUB = 1;
static const uint8_t VERSION_PATCH = 0;

enum COLOR_IDS {
	RESET, // 0

	RED, // r
	GREEN, // g
	YELLOW, // y
	BLUE, // b
	MAGENTA, // m
	CYAN, // c
	WHITE, // w

	BOLD_RED, // br
	BOLD_GREEN, // bg
	BOLD_YELLOW, // by
	BOLD_BLUE, // bb
	BOLD_MAGENTA, // bm
	BOLD_CYAN, // bc
	BOLD_WHITE, // bw

	PRIMARY, // p
	SECONDARY, // s
	TERTIARY, // t
	EXTRA, // e

	LAST,
};

extern const char * COLORS[ LAST ];

// created to allow colored display using string interpolation
// and printf like syntax for variables
// option values are:
//   0 -> stdout
//   1 -> stderr
//   2 -> test ( no display )
void _display( const int option, const char * fmt, ... );

// these definitions allow for various use scenarios of _display function
// if we use fmt parameter explicitly here, it will cause error if there is no
// variable argument to be used ( unless we opt for the gcc extension: ##__VA_ARGS__ )
// so, instead, we just let it use fmt as a variadic argument as well
#define display( ... ) _display( 0, __VA_ARGS__ )
#define display_err( ... ) _display( 1, __VA_ARGS__ )
#define display_test( ... ) _display( 2, __VA_ARGS__ )

#endif // CORE_H
