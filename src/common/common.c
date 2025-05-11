#include "common.h"

#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <time.h>

/* mkdir -p equivalent */
int common_mkdir_p(const char *path) {
    char tmp[PATH_MAX];
    strncpy(tmp, path, sizeof(tmp));
    tmp[sizeof(tmp)-1] = '\0';
    for (char *p = tmp + 1; *p; p++) {
        if (*p == '/') {
            *p = '\0';
            if (mkdir(tmp, 0755) && errno != EEXIST)
                return -1;
            *p = '/';
        }
    }
    if (mkdir(tmp, 0755) && errno != EEXIST)
        return -1;
    return 0;
}

/* Helper: does `name` end with `ext`? */
static int ends_with(const char *name, const char *ext) {
    size_t nl = strlen(name);
    size_t el = strlen(ext);
    if (nl < el) return 0;
    return strcmp(name + nl - el, ext) == 0;
}

/**
 * Pick a random file in `dir` ending with any extension in `exts[]`.
 */
char *common_pick_random_file(const char *dir, const char *exts[]) {
    DIR *d = opendir(dir);
    if (!d) return NULL;
    struct dirent *ent;
    char **list = NULL;
    size_t count = 0;

    /* Collect matching names */
    while ((ent = readdir(d))) {
        for (const char **e = exts; *e; ++e) {
            if (ends_with(ent->d_name, *e)) {
                char *name = strdup(ent->d_name);
                if (!name) break;
                char **tmp = realloc(list, (count + 1) * sizeof(*list));
                if (!tmp) {
                    free(name);
                    break;
                }
                list = tmp;
                list[count++] = name;
                break;  /* don’t add same file twice if ext-list overlaps */
            }
        }
    }
    closedir(d);

    if (count == 0) {
        free(list);
        return NULL;
    }

    /* Seed rand once */
    srand((unsigned)time(NULL));

    size_t idx = (size_t)rand() % count;
    size_t len = strlen(dir) + 1 + strlen(list[idx]) + 1;
    char *result = malloc(len);
    if (result) {
        snprintf(result, len, "%s/%s", dir, list[idx]);
    }

    for (size_t i = 0; i < count; i++)
        free(list[i]);
    free(list);

    return result;
}
