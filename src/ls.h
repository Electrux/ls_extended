/*
	Copyright (c) 2019, Electrux
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

#include "core.h"

typedef struct
{
	char name[ MAX_STR_LEN ];
	char ext[ 30 ];
	char icon[ 30 ];
	char size[ 30 ];
	int width;
	struct stat st;
	bool lnk_is_dead;
	int lnk_jumps;
	char lnk_loc[ MAX_STR_LEN ];
	struct stat lnk_st;
} stat_info_t;

typedef struct
{
	int name;
	int user;
	int grp;
	int size;
	int inode;
	int links;
} max_lens_t;


int ls( const struct winsize * ws, const char * loc, size_t flags, int loc_count );

#endif // LS_H