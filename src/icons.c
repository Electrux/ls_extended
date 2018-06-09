/*
	Copyright (c) 2018, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#include <stdarg.h>
#include <string.h>

#include "../include/core.h"

#include "../include/icons.h"

static const char * get_file_icon_by_ext( const char * ext );
static const char * get_file_icon_by_name( const char * name );

static int is( const char * of, const int count, ... );
static int contains( const char * of, const int count, ... );

#define IS( of, ... ) is( of, PP_NARG( __VA_ARGS__ ), __VA_ARGS__)
#define CONTAINS( of, ... ) contains( of, PP_NARG( __VA_ARGS__ ), __VA_ARGS__ )

// Default file icon is text file
#define DEFAULT_FILE_ICON "\uf15c"
// Default directory icon is normal directory
#define DEFAULT_DIR_ICON  "\uf07c"

const char * get_file_icon( const char * file )
{
	char name[ 1000 ], ext[ 100 ];
	split_file( file, name, ext );
	if( strcmp( ext, "\0" ) != 0 ) {
		const char * ext_icon = get_file_icon_by_ext( ext );
		if( strcmp( ext_icon, DEFAULT_FILE_ICON ) != 0 ) return ext_icon;
	}
	return get_file_icon_by_name( name );
}

const char * get_dir_icon( const char * dir )
{
	if( CONTAINS( dir, ".git" ) ) return "\ue5fd";

	return DEFAULT_DIR_ICON;
}

static const char * get_file_icon_by_ext( const char * ext )
{
	// OS

	// Apple
	if( IS( ext, "DS_Store" ) ) return "\ue711";

	// Configuration

	else if( IS( ext, "json" ) ) return "\ufb25";
	else if( IS( ext, "lock" ) ) return "\uf023";
	else if( IS( ext, "ini", "yaml", "yml" ) ) return "\ue615";

	// Data handling / manipulation

	else if( IS( ext, "html" ) ) return "\ue60e";
	else if( IS( ext, "md" ) ) return "\ue73e";

	// Git

	else if( CONTAINS( ext, "git" ) ) return "\uf7a3";

	// Terminal stuff

	else if( CONTAINS( ext, "zsh", "bash" ) ) return "\ue615";
	else if( CONTAINS( ext, "vim" ) ) return "\ue7c5";

	// Languages

	// C, C++
	else if( IS( ext, "h", "hh", "hpp", "hxx", "h++" ) ) return "\ufd27";
	else if( IS( ext, "c" ) ) return "\ufb70";
	else if( IS( ext, "cc", "cpp", "cxx", "c++" ) ) return "\ufb71";
	// C#
	else if( IS( ext, "cs" ) ) return "\uf81a";
	// Closure
	else if( IS( ext, "clj", "cljs", "cljc", "edn" ) ) return "\ue76a";
	// Coffeescript
	else if( IS( ext, "coffee", "litcoffee" ) ) return "\ue751";
	// TODO: Start working on languages starting with D

	// Elixir
	else if( IS( ext, "ex", "exs", "eex" ) ) return "\ue62d";

	return DEFAULT_FILE_ICON;
}

static const char * get_file_icon_by_name( const char * name )
{

	return DEFAULT_FILE_ICON;
}

static int is( const char * of, const int count, ... )
{
	va_list args;
	va_start( args, count );

	int ret_val = 0;

	for( int i = 0; i < count; ++i ) {
		const char * test_param = ( const char * )va_arg( args, const char * );
		if( strcmp( of, test_param ) == 0 ) {
			ret_val = 1;
			break;
		}
	}

	va_end( args );
	return ret_val;
}

static int contains( const char * of, const int count, ... )
{
	va_list args;
	va_start( args, count );

	int ret_val = 0;

	for( int i = 0; i < count; ++i ) {
		const char * test_param = ( const char * )va_arg( args, const char * );
		if( strncmp( of, test_param, strlen( test_param ) ) == 0 ) {
			ret_val = 1;
			break;
		}
	}

	va_end( args );
	return ret_val;
}
