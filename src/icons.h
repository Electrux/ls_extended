/*
	Copyright (c) 2019, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.
	Please read that file and understand the license terms
	before using or altering the project.
*/

#ifndef ICONS_H
#define ICONS_H

#include <stdbool.h>

const char * get_file_icon( const char * name, const char * ext, const bool is_link );
const char * get_dir_icon( const char * dir, const bool is_link );

#endif // ICONS_H