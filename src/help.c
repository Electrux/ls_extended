/*
	Copyright (c) 2019, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project"s root directory.
	Please read that file and understand the license terms
	before using or altering the project.
*/

#include "help.h"

#include <stdio.h>

#include "core.h"

#define OPT_SPACES 12

void disp_help(const char *prog_name)
{
	disp(stdout, "{p}Usage{0}: {s}%s {0}[{t}OPTIONS{0}] {t}LOCATION{0}\n\n", prog_name);

	disp(stdout, "{p}Options:\n\n");

	disp(stdout, "{s}%*s{0} - {t}Show this message and exit\n", OPT_SPACES, "--help");
	disp(stdout, "{s}%*s{0} - {t}disp version information and exit\n", OPT_SPACES, "--version");

	disp(stdout, "\n");

	disp(stdout,
	     "{s}%*s{0} - {t}Shows all files{0}/{t}folders{0}, {t}including the hidden ones{0}\n",
	     OPT_SPACES, "-a");
	disp(stdout,
	     "{s}%*s{0} - {t}Shows all files{0}/{t}folders{0}, {t}including the hidden{0},"
	     " {t}but excluding the {0}. {t}and {0}.. {t}ones{0}\n",
	     OPT_SPACES, "-A");
	disp(stdout, "{s}%*s{0} - {t}Shows only directories{0}\n", OPT_SPACES, "-d");
	disp(stdout, "{s}%*s{0} - {t}Shows only files{0}\n", OPT_SPACES, "-f");
	disp(stdout,
	     "{s}%*s{0} - {t}Shows all sizes in human readable format{0}, {e}can only be used with "
	     "the {s}l {e}option{0}\n",
	     OPT_SPACES, "-h");
	disp(stdout,
	     "{s}%*s{0} - {t}Shows inode numbers of all files{0}/{t}folders{0}, {e}can only be "
	     "used with the {s}l {e}option{0}\n",
	     OPT_SPACES, "-i");
	disp(stdout,
	     "{s}%*s{0} - {t}Shows all files{0}/{t}directories in a list form with various "
	     "information about them{0}\n",
	     OPT_SPACES, "-l");

	disp(stdout, "{s}%*s{0} - {t}Sorts the output by directories first{0}\n", OPT_SPACES, "-s");
	disp(stdout,
	     "{s}%*s{0} - {t}{0}({e}The numeric digit 'one'{0}) {t}Force output to be one entry "
	     "per line{0}\n",
	     OPT_SPACES, "-1");
}