/*
	Copyright (c) 2018, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#ifndef DIRCOLORS_H
#define DIRCOLORS_H

#include <stdbool.h>

#define MAX_DIR_COLORS 512

enum E_TYPE
{
	T_FILE,
	T_EXT
};

struct dircolor
{
	E_TYPE type;
	char * name;
	char * col_code;
};

struct dircolors
{
	struct dircolor * colors;
};

static bool dir_colors_is_loaded = false;
static struct dircolors dircols;

bool load_dircolors();

bool is_loaded_dircolors();

char * get_dircol_entry(const E_TYPE type, const char * name);

bool unload_dircolors();

#endif // DIRCOLORS_H
