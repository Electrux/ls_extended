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
	display_test( "{r}Red{0}, {g}Green{0}, {y}Yellow{0}, {b}Blue{0}, {m}Magenta{0}, {c}Cyan{0}, {w}White{0}\n" );
	display_test( "{br}Bold Red{0}, {bg}Bold Green{0}, {by}Bold Yellow{0}, {bb}Bold Blue{0}, {bm}Bold Magenta{0}, {bc}Bold Cyan{0}, {bw}Bold White{0}\n" );
	return 0;
}
