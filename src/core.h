/*
	Copyright (c) 2019, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.
	Please read that file and understand the license terms
	before using or altering the project.
*/

#ifndef CORE_H
#define CORE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static const int VERSION_MAIN = 2;
static const int VERSION_SUB = 0;
static const int VERSION_PATCH = 0;

#define MAX_STR_LEN 1025

enum ERRORS
{
	SUCCESS,
	LOC_NOT_OPENED,
	VEC_INIT_FAIL,
};

void split_file( const char * file, char * name, char * ext );

// created to allow colored display using string interpolation
// and printf like syntax for variables
void disp( FILE * out, const char * fmt, ... );
// enable or disable colorization
bool * disp_cols();

int numlen( unsigned long long num );

uint8_t extra_space_count( const char * str, const int used_bytes );
uint64_t get_extra_spaces( const char * str );

size_t utf8_strlen( const char * _s );

#endif // CORE_H