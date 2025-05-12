// tests/test_common.c

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "../src/common/common.h"

// Utility: recursively remove a directory
static void rmrf(const char *path) {
    char cmd[512];
    snprintf(cmd, sizeof cmd, "rm -rf \"%s\"", path);
    system(cmd);
}

int main(void) {
    // 1) Test mkdir_p on a nested path
    const char *base = "/tmp/synapse_common_test";
    const char *nested = "/tmp/synapse_common_test/a/b/c";
    rmrf(base);
    assert(common_mkdir_p(nested) == 0);

    // verify it exists
    DIR *d = opendir(nested);
    assert(d != NULL);
    closedir(d);

    printf("test_common: mkdir_p PASS\n");

    // 2) Test pick_random_file
    const char *tdir = "/tmp/synapse_common_test/files";
    rmrf(base);
    common_mkdir_p(tdir);

    // create three dummy files
    const char *filenames[3] = { "one.txt", "two.txt", "three.txt" };
    for (int i = 0; i < 3; i++) {
        char path[256];
        snprintf(path, sizeof path, "%s/%s", tdir, filenames[i]);
        FILE *f = fopen(path, "w");
        assert(f);
        fprintf(f, "hello\n");
        fclose(f);
    }

    // pick random .txt
    srand(0);
    const char *exts[] = { ".txt", NULL };
    char *picked = common_pick_random_file(tdir, exts);
    
    assert(picked);
    // ensure it ends with one of our filenames
    int ok = 0;
    for (int i = 0; i < 3; i++) {
        if (strstr(picked, filenames[i])) { ok = 1; break; }
    }
    assert(ok);
    free(picked);

    printf("test_common: pick_random_file PASS\n");

    // cleanup
    rmrf(base);
    return 0;
}
