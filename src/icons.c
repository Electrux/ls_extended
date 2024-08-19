// The checking is NOT case sensitive

#include "icons.h"

#include <stdarg.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>

// Default file icon is text file or normal link file
#define DEFAULT_FILE_ICON "\uf15c"
#define DEFAULT_LINK_FILE_ICON "\uf481"
// Default directory icon is normal directory or normal link directory
#define DEFAULT_DIR_ICON "\uf07c"
#define DEFAULT_LINK_DIR_ICON "\uf482"

static const char *get_file_icon_by_ext(const char *ext, const bool is_link);
static const char *get_file_icon_by_name(const char *name, const bool is_link);

static bool find_in(const char *of, const char *values, bool is_exact_match);

#define IS(of, values) find_in(of, values, true)
#define BEGINS(of, values) find_in(of, values, false)

#define BITMAP_IMAGE                                                           \
    "png, jpg, jpeg, gif, webp, bmp, ppm, pgm, pbm, pnm, tiff, ico, xbm, xpm" \
	"tga, avif, jp2, j2k, jxl, qoi, ktx, dds, wal"
#define VECTOR_IMAGE "svg, ai, eps"

#define LOSSLESS_AUDIO                                                                          \
	"8svx, iff, aiff, aif, aifc, au, snd, bwf, wav, wave, cdda, raw, pcm, sam, ra, ram, "   \
	"flac, la, pac, ape, ofr, ofs, off, rka, shn, tak, tta, wv, brstm, dts, dtshd, dtsma, " \
	"ast, aw, psf, alac, m4a"
#define LOSSY_AUDIO                                                                             \
	"amr, 3ga, mp1, mp2, mp3, spx, gsm, wma, aac, mpc, vqf, ots, swa, vox, voc, dwd, smp, " \
	"ogg, oga, opus"
#define PLAYLIST "cue, aimppl, asx, xpl, xspf, zpl, m3u, m3u8, pls"

#define VIDEO "avi, flv, mkv, mov, mp4, ogv, webm"

#define MODEL_3D 															  \
	"obj, gltf, glb, blend, 3ds, ase, dae, vta, mdl, md2, md3, mdc, md5mesh, " \
	"md5anim, md5camera, irr, irrmesh, ply, ac3d, vtk"

static const bool is_default_icon(const char *icon)
{
	return strcmp(icon, DEFAULT_FILE_ICON) == 0 || strcmp(icon, DEFAULT_LINK_FILE_ICON) == 0;
}

const char *get_file_icon(const char *name, const char *ext, const bool is_link)
{
	const char *file_icon = get_file_icon_by_name(name, is_link);
	if(strcmp(ext, "\0") != 0 && is_default_icon(file_icon)) {
		const char *ext_icon = get_file_icon_by_ext(ext, is_link);
		return ext_icon;
	}
	return file_icon;
}

const char *get_dir_icon(const char *dir, const bool is_link)
{
	if(BEGINS(dir, ".git, git")) return "\ue5fb";
	else if(BEGINS(dir, ".trash, trash")) return "\uf1f8";
	else if(BEGINS(dir, ".bash")) return "\ue795";
	else if(BEGINS(dir, ".bundle, .gem")) return "\ue739";
	else if(BEGINS(dir, ".cargo, .rustup")) return "\ue7a8";
	else if(BEGINS(dir, ".config")) return "\ue5fc";
	else if(BEGINS(dir, ".docker")) return "\uf308";
	else if(BEGINS(dir, ".go")) return "\ue626";
	else if(IS(dir, ".idea")) return "\ue7b5";
	else if(BEGINS(dir, ".mix")) return "\ue62d";
	else if(BEGINS(dir, ".npm, node_modules")) return "\ue5fa";
	else if(BEGINS(dir, ".vs, .vscode")) return "\ue70c";
	else if(BEGINS(dir, ".weechat")) return "\uf448";
	else if(BEGINS(dir, "application")) return "\uf2d0";
	else if(BEGINS(dir, "bin, config")) return "\ue5fc";
	else if(BEGINS(dir, "desktop")) return "\uf108";
	else if(BEGINS(dir, "download")) return "\uf409";
	else if(BEGINS(dir, "library")) return "\uf02e";
	else if(BEGINS(dir, "movie")) return "\uf008";
	else if(BEGINS(dir, "music, songs, audio")) return "\uf001";
	else if(BEGINS(dir, "photo, picture")) return "\uf03e";
	else if(IS(dir, "__pycache__")) return "\ue606";
	else if(BEGINS(dir, "video")) return "\uf03d";

	if(is_link) return DEFAULT_LINK_DIR_ICON;
	return DEFAULT_DIR_ICON;
}

static const char *get_file_icon_by_ext(const char *ext, const bool is_link)
{
	// OS

	// Apple
	if(IS(ext, "DS_Store")) return "\ue711";

	// Configuration

	else if(IS(ext, "json")) return "\ue60b";
	else if(IS(ext, "lock")) return "\uf023";
	else if(IS(ext, "ini, yaml, yml, env, toml, conf, cfg")) return "\ue615";

	// Data handling / manipulation

	else if(IS(ext, "html")) return "\ue60e";
	else if(IS(ext, "md")) return "\ue73e";
	else if(IS(ext, "xml, ui")) return "\ue796";

	// Git

	else if(BEGINS(ext, "git")) return "\ue702";

	// Terminal stuff

	else if(IS(ext, "zsh, bash, sh, ps1")) return "\ue795";
	else if(BEGINS(ext, "zsh, bash")) return "\ue615";
	else if(BEGINS(ext, "vim")) return "\ue7c5";

	// Databases
	else if(IS(ext, "sql, sqlite, db")) return "\ue706";

	// Archive
	else if(IS(ext, "zip, rar, 7z, tar, a, ar, cab, pk3")) return "\uf187";
	
	// Fonts
	else if(IS(ext, "ttf, otf, woff, woff2, eot")) return "\ue659";

    // Image
    else if(IS(ext, BITMAP_IMAGE)) return "\ue60d";
    else if(IS(ext, VECTOR_IMAGE)) return "\uee24";

	// Audio
	// lossless
	else if(IS(ext, LOSSLESS_AUDIO)) return "\uf001";
	// lossy
	else if(IS(ext, LOSSY_AUDIO)) return "\uf001";
	// playlist
	else if(IS(ext, PLAYLIST)) return "\uf04b";

	// Video
	else if(IS(ext, VIDEO)) return "\uf03d";

	// 3d Models and related
	else if(IS(ext, MODEL_3D)) return "\uf1b2";

	// Languages

	// C, C++
	else if(IS(ext, "h, hh,hpp,hxx, h++")) return "\uf0fd";
	else if(IS(ext, "c")) return "\ue649";
	else if(IS(ext, "cc, cpp, cxx, c++")) return "\ue646";
	// C#
	else if(IS(ext, "cs")) return "\ue648";
	// Closure
	else if(IS(ext, "clj, cljs, cljc, edn")) return "\ue76a";
	// Coffeescript
	else if(IS(ext, "coffee, litcoffee")) return "\ue751";
	// Elixir
	else if(IS(ext, "ex, exs, eex")) return "\ue62d";
	// Elm
	else if(IS(ext, "elm")) return "\ue62c";
	// Erlang
	else if(IS(ext, "erl, hrl")) return "\ue7b1";
	// F-Sharp
	else if(IS(ext, "fs, fsi, fsscript")) return "\ue7a7";
	// Go
	else if(IS(ext, "go")) return "\ue626";
	// Haskell
	else if(IS(ext, "hs, lhs")) return "\ue777";
	// Java
	else if(IS(ext, "java, class, jar")) return "\ue738";
	// JS
	else if(IS(ext, "js")) return "\ue74e";
	// Julia
	else if(IS(ext, "jl")) return "\ue624";
	// Lua
	else if(IS(ext, "lua")) return "\ue620";
	// Perl
	else if(IS(ext, "pl, pm, t, pod")) return "\ue769";
	// PHP
	else if(IS(ext, "php, php3, php4, php5, phtml, phar")) return "\ue73d";
	// Prolog
	else if(IS(ext, "pro, P")) return "\ue7a1";
	// Python
	else if(IS(ext, "py, pyc, pyd, pyx")) return "\ue606";
	// Ruby
	else if(IS(ext, "rb")) return "\ue739";
	// Rust
	else if(IS(ext, "rs, rlib")) return "\ue7a8";
	// Scala
	else if(IS(ext, "scala, sc")) return "\ue737";
	// Switft
	else if(IS(ext, "swift")) return "\ue755";
	// Typescript
	else if(IS(ext, "ts, tsx")) return "\ue69d";
	// Qt stuff
	else if(IS(ext, "qml, qrc, qsb, qss")) return "\uf375";

	// Misc.
	else if(IS(ext, "pdf")) return "\ue67d";
	else if(IS(ext, "torrent")) return "\uf0ec";
	else if(IS(ext, "log")) return "\uf1ea";
	else if(IS(ext, "exe")) return "\ue70f";
	else if(IS(ext, "cache")) return "\uf49b";
	else if(IS(ext, "sln")) return "\ue70c";
	else if(IS(ext, "desktop")) return "\uf108";

	if(is_link) return DEFAULT_LINK_FILE_ICON;
	return DEFAULT_FILE_ICON;
}

static const char *get_file_icon_by_name(const char *name, const bool is_link)
{
	// Development
	if(BEGINS(name, "Makefile, CMake")) return "\ue673";
	else if(BEGINS(name, "Dockerfile, docker-compose")) return "\uf308";

	// Other
	if(BEGINS(name, "LICENSE, license, copying, COPYING")) return "\uf2c2";
	else if(BEGINS(name, "AUTHOR, author, CONTRIBUTOR, contributor")) return "\uf4fd";
	else if(IS(name, "config")) return "\ue615";
	else if(IS(name, "TODO, todo, TODO.txt, todo.txt")) return "\ue69c";
	// TODO:
	if(is_link) return DEFAULT_LINK_FILE_ICON;
	return DEFAULT_FILE_ICON;
}

static inline bool is_whitespc(const char c) { return c == ',' || c == ' ' || c == '\0'; }

static bool find_in(const char *of, const char *values, bool is_exact_match)
{
	int len_val = strlen(values);
	char tmp_str[100];
	int tmp_ctr = 0;
	for(int i = 0; i <= len_val; ++i) {
		if(is_whitespc(values[i])) {
			if(tmp_ctr == 0) continue;
			tmp_str[tmp_ctr] = '\0';
			if(is_exact_match) {
				if(strcasecmp(of, tmp_str) == 0) return true;
			} else {
				if(strncasecmp(of, tmp_str, strlen(tmp_str)) == 0) return true;
			}
			strcpy(tmp_str, "\0");
			tmp_ctr = 0;
			continue;
		}
		tmp_str[tmp_ctr++] = values[i];
	}
	return false;
}
