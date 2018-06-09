/*
	Copyright (c) 2018, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#ifndef LS_H
#define LS_H

#include <stdlib.h>
#include <sys/ioctl.h>

int ls( const struct winsize * ws, const char * loc, const size_t flags );

#endif // LS_H
