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

	fprintf( OUT_LOC, "Deleting at 7th position\n" );
	int res1 = str_vec_rem_loc( strs, 8 );

	fprintf( OUT_LOC, "=> Result = %d, New size = %zu\n", res1, str_vec_get_count( strs ) );

	fprintf( OUT_LOC, "Data at 2nd position = %s\n", str_vec_get( strs, 3 ) );

	fprintf( OUT_LOC, "Data count = %zu\n", str_vec_get_count( strs ) );

	str_vec_delete( & strs );
	return 0;
}
