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
#include <stdbool.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

struct stat_info
{
	struct stat st;
	bool lnk_is_dead;
	int lnk_jumps;
	char lnk_loc[ 1024 ];
	struct stat lnk_st;
};

int ls( const struct winsize * ws, const char * loc, const size_t flags, int loc_count );

#endif // LS_H
