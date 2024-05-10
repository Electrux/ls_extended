/*
	Copyright (c) 2019, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.
	Please read that file and understand the license terms
	before using or altering the project.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "cmdopts.h"
#include "core.h"
#include "help.h"
#include "ls.h"
#include "sorts.h"
#include "vec.h"

int main(int argc, char **argv)
{
	if(argc > 1 && strcmp(argv[1], "--help") == 0) {
		disp_help(argv[0]);
		return 0;
	}

	if(argc > 1 && strcmp(argv[1], "--version") == 0) {
		disp(stdout, "{p}ls extended version{0}: {s}%d{0}.{s}%d{0}.{s}%d{0}\n",
		     VERSION_MAIN, VERSION_SUB, VERSION_PATCH);
		return 0;
	}

	vec_t *locs  = vec_create(0, NULL);
	size_t flags = get_cmd_opts(argc, (const char **)argv, locs);

	if(flags & OPT_CAPS_N) {
		*disp_cols() = false;
	}

	if(flags & OPT_V) {
		disp(stdout, "{p}ls extended version{0}: {s}%d{0}.{s}%d{0}.{s}%d{0}\n",
		     VERSION_MAIN, VERSION_SUB, VERSION_PATCH);
		return 0;
	}

	set_rev_sort(flags & OPT_R);

	struct winsize ws;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);

	int res		 = 0;
	size_t loc_count = vec_count(locs);
	for(size_t i = 0; i < loc_count; ++i) {
		res = ls(&ws, (const char *)vec_get_data(locs, i), flags, loc_count);
		if(res != 0) break;
	}
	vec_destroy(&locs);
	return 0;
}
