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

#define PP_NARG( ... )				\
	PP_NARG_( __VA_ARGS__, PP_RSEQ_N() )
#define PP_NARG_(...)				\
	PP_ARG_N( __VA_ARGS__ )
#define PP_ARG_N(							\
		 _1,  _2,  _3,  _4,  _5,  _6,  _7,  _8,  _9, _10,	\
		_11, _12, _13, _14, _15, _16, _17, _18, _19, _20,	\
		_21, _22, _23, _24, _25, _26, _27, _28, _29, _30,	\
		_31, _32, _33, _34, _35, _36, _37, _38, _39, _40,	\
		_41, _42, _43, _44, _45, _46, _47, _48, _49, _50,	\
		_51, _52, _53, _54, _55, _56, _57, _58, _59, _60,	\
		_61, _62, _63,   N, ... ) N

#define PP_RSEQ_N()				\
	63, 62, 61, 60,				\
	59, 58, 57, 56, 55, 54, 53, 52, 51, 50,	\
	49, 48, 47, 46, 45, 44, 43, 42, 41, 40,	\
	39, 38, 37, 36, 35, 34, 33, 32, 31, 30,	\
	29, 28, 27, 26, 25, 24, 23, 22, 21, 20,	\
	19, 18, 17, 16, 15, 14, 13, 12, 11, 10,	\
	 9,  8,  7,  6,  5,  4,  3,  2,  1,  0

static const uint8_t VERSION_MAIN = 0;
static const uint8_t VERSION_SUB = 1;
static const uint8_t VERSION_PATCH = 0;

enum ERRORS {
	SUCCESS,
	LOC_NOT_OPENED,
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

#endif // CORE_H
