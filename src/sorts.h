#ifndef SORTS_H
#define SORTS_H

#include <stdbool.h>

void set_rev_sort(bool r_sort);
int get_rev_sort();

int sort_name(const void *a, const void *b);
int sort_ext(const void *a, const void *b);
int sort_mtime(const void *a, const void *b);
int sort_size(const void *a, const void *b);

#endif // SORTS_H