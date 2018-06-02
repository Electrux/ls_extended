/*
	Copyright (c) 2018, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#include <stdio.h>

#include "../include/core.h"

int main()
{
	display_test( "Variadic: %s, %c, %d, %f\n", "str", 'c', 10, 12.4 );
	return 0;
}
