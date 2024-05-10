#include "sorts.h"

#include <string.h>

#include "ls.h"

static bool rev_sort = false;

void set_rev_sort(bool r_sort) { rev_sort = r_sort; }

int get_rev_sort() { return rev_sort; }

int sort_name(const void *a, const void *b)
{
	const stat_info_t *aa = *(const stat_info_t **)a;
	const stat_info_t *bb = *(const stat_info_t **)b;
	if(get_rev_sort()) return strcmp(bb->name, aa->name);
	return strcmp(aa->name, bb->name);
}

int sort_ext(const void *a, const void *b)
{
	const stat_info_t *aa = *(const stat_info_t **)a;
	const stat_info_t *bb = *(const stat_info_t **)b;
	if(get_rev_sort()) return strcmp(bb->ext, aa->ext);
	return strcmp(aa->ext, bb->ext);
}

int sort_mtime(const void *a, const void *b)
{
	const stat_info_t *aa = *(const stat_info_t **)a;
	const stat_info_t *bb = *(const stat_info_t **)b;
#ifdef __linux__
	if(get_rev_sort()) return bb->lnk_st.st_mtim.tv_sec - aa->lnk_st.st_mtim.tv_sec;
	return aa->lnk_st.st_mtim.tv_sec - bb->lnk_st.st_mtim.tv_sec;
#else
	if(get_rev_sort()) return bb->lnk_st.st_mtimespec.tv_sec - aa->lnk_st.st_mtimespec.tv_sec;
	return aa->lnk_st.st_mtimespec.tv_sec - bb->lnk_st.st_mtimespec.tv_sec;
#endif
}

int sort_size(const void *a, const void *b)
{
	const stat_info_t *aa = *(const stat_info_t **)a;
	const stat_info_t *bb = *(const stat_info_t **)b;
	if(get_rev_sort()) return bb->lnk_st.st_size - aa->lnk_st.st_size;
	return aa->lnk_st.st_size - bb->lnk_st.st_size;
}
