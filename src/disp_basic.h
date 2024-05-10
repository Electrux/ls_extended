#ifndef DISP_BASIC_H
#define DISP_BASIC_H

#include "ls.h"
#include "vec.h"

void disp_basic(vec_t *locs, const struct winsize *ws, max_lens_t *maxlens, const size_t flags);

#endif // DISP_BASIC_H