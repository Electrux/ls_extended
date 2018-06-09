/*
	Copyright (c) 2018, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

// Apparently, strdup must be explicitly enabled -_-
#if defined __linux__ && defined __GNUC__ && !defined __clang__ && defined __STDC_ALLOC_LIB__
#define __STDC_WANT_LIB_EXT2__ 1
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/strvec.h"

#define CHECK_NULL_VEC( vec, ret_val )					\
	if( vec == NULL ) {						\
		fprintf( stderr, "Attempted to use NULL vector\n" );	\
		return ret_val;						\
	}

// Used to compare strings for sorting
static int compare_str( const void * a, const void * b );

struct str_vec * str_vec_create()
{
	struct str_vec * temp = ( struct str_vec * )malloc( sizeof( struct str_vec ) );
	temp->total = 1;
	temp->count = 0;
	temp->data = ( char ** )malloc( sizeof( char * ) * temp->total );

	return temp;
}

int str_vec_delete( struct str_vec ** vec )
{
	CHECK_NULL_VEC( vec, -2 );
	CHECK_NULL_VEC( * vec, -2 );
	for( size_t i = 0; i < ( * vec )->count; ++i ) {
		free( ( * vec )->data[ i ] );
	}
	free( ( * vec )->data );
	free( * vec );

	* vec = NULL;
	return 0;
}

int str_vec_add( struct str_vec * vec, const char * str )
{
	CHECK_NULL_VEC( vec, -2 );

	if( vec->count >= vec->total ) {
		vec->total *= 2;
		vec->data = ( char ** )realloc( vec->data, sizeof( char * ) * vec->total );
	}

	vec->data[ vec->count++ ] = strdup( str );
	return vec->count;
}

int str_vec_add_vec( struct str_vec * dest, const struct str_vec * src )
{
	CHECK_NULL_VEC( dest, -2 );
	CHECK_NULL_VEC( src, -2 );

	for( size_t i = 0; i < src->count; ++i ) {
		str_vec_add( dest, src->data[ i ] );
	}

	return dest->count;
}

int str_vec_rem_loc( struct str_vec * vec, const size_t loc )
{
	CHECK_NULL_VEC( vec, -2 );

	if( loc >= vec->count ) {
		fprintf( stderr, "Provided loc %zu, max index is %zu\n", loc, vec->count - 1 );
		return -1;
	}

	free( vec->data[ loc ] );

	for( size_t i = loc; i < vec->count - 1; ++i ) {
		vec->data[ i ] = vec->data[ i + 1 ];
	}
	vec->data[ --vec->count ] = NULL;

	if( vec->count > 0 && vec->count * 2 <= vec->total ) {
		vec->total /= 2;
		vec->data = ( char ** )realloc( vec->data, sizeof( char * ) * vec->total );
	}

	return vec->count;
}

int str_vec_rem_str( struct str_vec * vec, const char * str )
{
	CHECK_NULL_VEC( vec, -2 );

	int loc = -1;

	for( size_t i = 0; i < vec->count - 1; ++i ) {
		if( strcmp( vec->data[ i ], str ) == 0 ) {
			loc = i;
			break;
		}
	}

	return loc < 0 ? loc : str_vec_rem_loc( vec, loc );
}

const char * str_vec_get( const struct str_vec * vec, const size_t loc )
{
	CHECK_NULL_VEC( vec, NULL );

	if( loc >= vec->count ) {
		fprintf( stderr, "Provided loc %zu, max index is %zu\n", loc, vec->count - 1 );
		return NULL;
	}

	return ( const char * )vec->data[ loc ];
}

char * str_vec_get_dup( const struct str_vec * vec, const size_t loc )
{
	CHECK_NULL_VEC( vec, NULL );

	if( loc >= vec->count ) {
		fprintf( stderr, "Provided loc %zu, max index is %zu\n", loc, vec->count - 1 );
		return NULL;
	}

	return strdup( vec->data[ loc ] );
}

size_t str_vec_get_count( const struct str_vec * vec )
{
	CHECK_NULL_VEC( vec, 0 );
	return vec->count;
}

void str_vec_sort( struct str_vec * vec )
{
	qsort( vec->data, vec->count, sizeof( const char * ), compare_str );
}

static int compare_str( const void * a, const void * b )
{
	return strcmp( * ( const char ** ) a, * ( const char ** ) b );
}
