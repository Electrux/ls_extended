#ifndef LS_H
#define LS_H

#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "core.h"

typedef struct
{
	char name[MAX_STR_LEN];
	char ext[513];
	char icon[30];
	char size[30];
	char user[200];
	char grp[200];
	int width;
	int namelen;
	int iconlen;
	struct stat st;
	bool lnk_is_dead;
	int lnk_jumps;
	char lnk_loc[MAX_STR_LEN];
	struct stat lnk_st;
} stat_info_t;

typedef struct
{
	int name;
	int user;
	int grp;
	int size;
	int inode;
	int links;
} max_lens_t;

int ls(const struct winsize *ws, const char *loc, size_t flags, int loc_count);

#endif // LS_H