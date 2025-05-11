#pragma once

#include <stddef.h>
#include "../datastructures/heap.h"

/**
 * mkdir -p equivalent.
 * Returns 0 on success, -1 on failure.
 */
int common_mkdir_p(const char *path);

/**
 * Pick a random file in `dir` whose name ends with any of the extensions
 * listed in `exts[]`.  `exts` must be a NULL-terminated array of C-strings.
 * Returns a heap-allocated full path (caller must free), or NULL if none found.
 */
char *common_pick_random_file(const char *dir, const char *exts[]);
