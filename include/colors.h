/*
	Copyright (c) 2018, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#ifndef COLORS_H
#define COLORS_H

enum COLOR_IDS {
	RESET, // 0

	RED, // r
	GREEN, // g
	YELLOW, // y
	BLUE, // b
	MAGENTA, // m
	CYAN, // c
	WHITE, // w

	BOLD_RED, // br
	BOLD_GREEN, // bg
	BOLD_YELLOW, // by
	BOLD_BLUE, // bb
	BOLD_MAGENTA, // bm
	BOLD_CYAN, // bc
	BOLD_WHITE, // bw

	PRIMARY, // p
	SECONDARY, // s
	TERTIARY, // t
	EXTRA, // e

	LAST,
};

extern const char * COLORS[ LAST ];

const char * get_color( const char * id );

#endif // COLORS_H
