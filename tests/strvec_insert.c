/*
	Copyright (c) 2018, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#include <stdio.h>

#include "../include/strvec.h"

#define OUT_LOC stdout

int main()
{
	struct str_vec * strs = str_vec_create();
	char * test = "Hello world";
	for( int i = 0; i < 10; ++i ) {
		str_vec_add( strs, test );
	}

	fprintf( OUT_LOC, "Entered data, count = %zu\n", str_vec_get_count( strs ) );

	str_vec_delete( & strs );
	return 0;
}
