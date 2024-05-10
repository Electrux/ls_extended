#ifndef ICONS_H
#define ICONS_H

#include <stdbool.h>

const char *get_file_icon(const char *name, const char *ext, const bool is_link);
const char *get_dir_icon(const char *dir, const bool is_link);

#endif // ICONS_H