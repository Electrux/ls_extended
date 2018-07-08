/*
	Copyright (c) 2018, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#include "../include/dircolors.h"

static bool file_exists( const char * file )
{
	if( access( file, F_OK ) != -1 ) {
		return true;
	}
	return false;
}

bool load_dircolors()
{
	if( getenv( "HOME" ) == NULL ) return false;
	char location[1024];
	strcpy( location, getenv( "HOME" ) );
	strcat( location, "/.dircolors" );

	if( !file_exists( location ) ) return false;

	FILE * file = fopen( location, "r" );
	if( !file ) return false;

	dir_colors_is_loaded = true;
	dircols.colors = malloc( sizeof( struct dircolor ) * MAX_DIR_COLORS);
	char * line = NULL;
	size_t len = 0;

	ssize_t read;
	size_t ctr = 0;
	while( ( read = getline( & line, & len, file ) ) != -1 ) {
		if( line[ 0 ] != '*' && line[ 0 ] != '.') continue;
		if( line[ 0 ] == '*') {
			dircols.colors[ ctr ].type = T_FILE;
			dircols.colors[ ctr ].col_code = 
		}
	}
}

bool is_loaded_dircolors()
{
	return dir_colors_is_loaded;
}

static void set_name_color( struct dircolor * col, const char * str )
{
	char tmp_name[ 100 ], tmp_col[ 100 ];
	int len = strlen( str );
	bool found = false;
	int name_ctr = 0, col_ctr = 0;
	// str[ 0 ] is asterisk or dot
	for( int i = 1; i < len; ++i ) {
		if( str[ i ] == ' ' ) {
			found = true;
			continue;
		}
		if( found ) tmp_col[ col_ctr++ ] = str[ i ];
		else tmp_name[ name_ctr++ ] = str[ i ];
	}

}

static char * fetch_col_code( const char * col_str )
{

}
