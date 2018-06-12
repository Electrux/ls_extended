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
#include <stdint.h>

static const uint8_t VERSION_MAIN = 1;
static const uint8_t VERSION_SUB = 0;
static const uint8_t VERSION_PATCH = 0;

enum ERRORS {
	SUCCESS,
	LOC_NOT_OPENED,
	STR_VEC_INIT_FAIL,
};

void split_file( const char * file, char * name, char * ext );

// created to allow colored display using string interpolation
// and printf like syntax for variables
// padding, if > 0 will allow right padding for the string to display
// option values are:
//   0 -> stdout
//   1 -> stderr
//   2 -> test ( no display )
void _display( const int padding, const int option, const char * fmt, ... );

// these definitions allow for various use scenarios of _display function
// if we use fmt parameter explicitly here, it will cause error if there is no
// variable argument to be used ( unless we opt for the gcc extension: ##__VA_ARGS__ )
// so, instead, we just let it use fmt as a variadic argument as well
#define display( ... ) _display( 0, 0, __VA_ARGS__ )
#define display_err( ... ) _display( 0, 1, __VA_ARGS__ )
#define display_test( ... ) _display( 0, 2, __VA_ARGS__ )
#define display_padded( padding, ... ) _display( padding, 0, __VA_ARGS__ )

size_t utf8_strlen( const char * _s );

#endif // CORE_H
