/*
	Copyright (c) 2018, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#ifndef STRVEC_H
#define STRVEC_H

#include <stdlib.h>

struct str_vec
{
	size_t total;
	size_t count;

	char ** data;
};

struct str_vec * str_vec_create();
int str_vec_delete( struct str_vec ** vec );

int str_vec_add( struct str_vec * vec, const char * str );
int str_vec_add_vec( struct str_vec * dest, const struct str_vec * src );

int str_vec_repl( struct str_vec * vec, const size_t loc, const char * new_str );

int str_vec_rem_loc( struct str_vec * vec, const size_t loc );
int str_vec_rem_str( struct str_vec * vec, const char * str );

const char * str_vec_get( const struct str_vec * vec, const size_t loc );
char * str_vec_get_dup( const struct str_vec * vec, const size_t loc );

size_t str_vec_get_count( const struct str_vec * vec );

void str_vec_sort( struct str_vec * vec );

#endif // STRVEC_H
