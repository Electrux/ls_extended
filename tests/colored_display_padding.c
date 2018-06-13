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
	display( "\n" );
	display_padded( 30, "globally padded 30" );
	display( "Testing globally padded 1\n" );
	display_padded( 30, "Globally padded 30 2" );
	display( "Testing globally padded 2\n" );

	display( "%*s %*d", 14, "padded 14", 5, 100 );
	display( "Testing padded 1\n" );
	display( "%*s %*f", 14, "padded 14 2", 5, 3.4 );
	display( "Testing padded 2\n" );
	return 0;
}
