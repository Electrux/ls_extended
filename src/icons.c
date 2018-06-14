/*
	Copyright (c) 2018, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

// The checking is NOT case sensitive

#include <stdarg.h>
#include <string.h>
#include <stdbool.h>

#include "../include/core.h"

#include "../include/icons.h"

// Default file icon is text file or normal link file
#define DEFAULT_FILE_ICON "\uf15c"
#define DEFAULT_LINK_FILE_ICON "\uf481"
// Default directory icon is normal directory or normal link directory
#define DEFAULT_DIR_ICON  "\uf07c"
#define DEFAULT_LINK_DIR_ICON "\uf482"


static const char * get_file_icon_by_ext( const char * ext, const bool is_link );
static const char * get_file_icon_by_name( const char * name, const bool is_link );

static bool find_in( const char * of, const char * values, bool is_exact_match );

#define IS( of, values ) find_in( of, values, true )
#define BEGINS( of, values ) find_in( of, values, false )

const char * get_file_icon( const char * file, const bool is_link )
{
	char name[ 1000 ], ext[ 100 ];
	split_file( file, name, ext );
	if( strcmp( ext, "\0" ) != 0 ) {
		const char * ext_icon = get_file_icon_by_ext( ext, is_link );
		if( strcmp( ext_icon, DEFAULT_FILE_ICON ) != 0 ) return ext_icon;
	}
	return get_file_icon_by_name( name, is_link );
}

const char * get_dir_icon( const char * dir, const bool is_link )
{
	if( BEGINS( dir, ".git, git" ) ) return "\ue5fd";
	else if( BEGINS( dir, ".trash, trash" ) ) return "\uf1f8";
	else if( BEGINS( dir, ".bash" ) ) return "\ue795";
	else if( BEGINS( dir, ".bundle, .gem" ) ) return "\ue739";
	else if( BEGINS( dir, ".cargo, .rustup" ) ) return "\ue7a8";
	else if( BEGINS( dir, ".config" ) ) return "\ue5fc";
	else if( BEGINS( dir, ".docker" ) ) return "\uf308";
	else if( BEGINS( dir, ".go" ) ) return "\ue626";
	else if( BEGINS( dir, ".mix" ) ) return "\ue62d";
	else if( BEGINS( dir, ".npm, node_modules" ) ) return "\ue719";
	else if( BEGINS( dir, ".vs, .vscode" ) ) return "\ue70c";
	else if( BEGINS( dir, ".weechat" ) ) return "\ufbee";
	else if( BEGINS( dir, "application" ) ) return "\ufb13";
	else if( BEGINS( dir, "desktop" ) ) return "\uf108";
	else if( BEGINS( dir, "download" ) ) return "\uf74c";
	else if( BEGINS( dir, "library" ) ) return "\uf830";
	else if( BEGINS( dir, "movie" ) ) return "\uf880";
	else if( BEGINS( dir, "music, songs, audio" ) ) return "\uf832";
	else if( BEGINS( dir, "photo, picture" ) ) return "\uf03e";
	else if( IS( dir, "__pycache__" ) ) return "\uf81f";

	if( is_link ) return DEFAULT_LINK_DIR_ICON;
	return DEFAULT_DIR_ICON;
}

static const char * get_file_icon_by_ext( const char * ext, const bool is_link )
{
	// OS

	// Apple
	if( IS( ext, "DS_Store" ) ) return "\ue711";

	// Configuration

	else if( IS( ext, "json" ) ) return "\ufb25";
	else if( IS( ext, "lock" ) ) return "\uf023";
	else if( IS( ext, "ini, yaml, yml, env, toml" ) ) return "\ue615";

	// Data handling / manipulation

	else if( IS( ext, "html" ) ) return "\ue60e";
	else if( IS( ext, "md" ) ) return "\ue73e";

	// Git

	else if( BEGINS( ext, "git" ) ) return "\uf7a3";

	// Terminal stuff

	else if( IS( ext, "zsh, bash, sh" ) ) return "\ue795";
	else if( BEGINS( ext, "zsh, bash" ) ) return "\ue615";
	else if( BEGINS( ext, "vim" ) ) return "\ue7c5";

	// Languages

	// C, C++
	else if( IS( ext, "h, hh,hpp,hxx, h++" ) ) return "\ufd27";
	else if( IS( ext, "c" ) ) return "\ufb70";
	else if( IS( ext, "cc, cpp, cxx, c++" ) ) return "\ufb71";
	// C#
	else if( IS( ext, "cs" ) ) return "\uf81a";
	// Closure
	else if( IS( ext, "clj, cljs, cljc, edn" ) ) return "\ue76a";
	// Coffeescript
	else if( IS( ext, "coffee, litcoffee" ) ) return "\ue751";
	// Elixir
	else if( IS( ext, "ex, exs, eex" ) ) return "\ue62d";
	// Elm
	else if( IS( ext, "elm" ) ) return "\ue62c";
	// Erlang
	else if( IS( ext, "erl, hrl" ) ) return "\ue7b1";
	// F-Sharp
	else if( IS( ext, "fs, fsi, fsscript" ) ) return "\ue7a7";
	// Go
	else if( IS( ext, "go" ) ) return "\ue626";
	// Haskell
	else if( IS( ext, "hs, lhs" ) ) return "\ue777";
	// Java
	else if( IS( ext, "java, class, jar" ) ) return "\ue738";
	// JS
	else if( IS( ext, "js" ) ) return "\ue74e";
	// Julia
	else if( IS( ext, "jl" ) ) return "\ue624";
	// Lua
	else if( IS( ext, "lua" ) ) return "\ue620";
	// TODO: start from N

	// Python
	else if( IS( ext, "py, pyc, pyd, pyx" ) ) return "\uf81f";

	if( is_link ) return DEFAULT_LINK_FILE_ICON;
	return DEFAULT_FILE_ICON;
}

static const char * get_file_icon_by_name( const char * name, const bool is_link )
{
	if( BEGINS( name, "LICENSE, license, copying, COPYING" ) ) return "\uf2c2";
	// TODO
	if( is_link ) return DEFAULT_LINK_FILE_ICON;
	return DEFAULT_FILE_ICON;
}

static inline bool is_whitespc( const char c ) { return c == ',' || c == ' '; }

static bool find_in( const char * of, const char * values, bool is_exact_match )
{
	int len_val = strlen( values );
	char tmp_str[ 100 ];
	int tmp_ctr = 0;
	for( int i = 0; i < len_val; ++i ) {
		if( is_whitespc( values[ i ] ) || i == len_val - 1 ) {
			if( tmp_ctr == 0 ) continue;
			if( i == len_val - 1 && !is_whitespc( values[ i ] ) ) tmp_str[ tmp_ctr++ ] = values[ i ];

			if( is_whitespc( values[ i ] ) && i < len_val - 1 && !is_whitespc( values[ i + 1 ] ) ) {
				tmp_str[ tmp_ctr++ ] = values[ i ];
				continue;
			}

			tmp_str[ tmp_ctr ] = '\0';
			if( is_exact_match ) {
				if( strcasecmp( of, tmp_str ) == 0 ) return true;
			}
			else {
				if( strncasecmp( of, tmp_str, strlen( tmp_str ) ) == 0 ) return true;
			}
			strcpy( tmp_str, "\0" );
			tmp_ctr = 0;
			continue;
		}

		tmp_str[ tmp_ctr++ ] = values[ i ];
	}

	return false;
}
