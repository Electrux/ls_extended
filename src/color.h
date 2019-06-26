/*
	Copyright (c) 2019, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.
	Please read that file and understand the license terms
	before using or altering the project.
*/

#ifndef COLOR_H
#define COLOR_H

enum COLOR_IDS {
	COL_RESET, // 0

	COL_RED, // r
	COL_GREEN, // g
	COL_YELLOW, // y
	COL_BLUE, // b
	COL_MAGENTA, // m
	COL_CYAN, // c
	COL_WHITE, // w

	COL_BOLD_RED, // br
	COL_BOLD_GREEN, // bg
	COL_BOLD_YELLOW, // by
	COL_BOLD_BLUE, // bb
	COL_BOLD_MAGENTA, // bm
	COL_BOLD_CYAN, // bc
	COL_BOLD_WHITE, // bw

	COL_PRIMARY, // p
	COL_SECONDARY, // s
	COL_TERTIARY, // t
	COL_EXTRA, // e

	COL_LAST,
};

extern const char * COLORS[ COL_LAST ];

const char * get_col( const char * id );

#endif // COLOR_H