#ifndef DISP_LONG_H
#define DISP_LONG_H

#include "ls.h"
#include "vec.h"

void disp_long(vec_t *locs, const struct winsize *ws, const max_lens_t *maxlens,
	       const size_t flags);

#endif // DISP_LONG_H