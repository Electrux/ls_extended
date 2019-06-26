/*
	Copyright (c) 2019, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.
	Please read that file and understand the license terms
	before using or altering the project.
*/

#include <string.h>

#include "ls.h"
#include "sorts.h"

int sort_name( const void * a, const void * b )
{
	const stat_info_t * aa = * ( const stat_info_t ** )a;
	const stat_info_t * bb = * ( const stat_info_t ** )b;
	return strcmp( aa->name, bb->name );
}