#include "common.h"
#include <stdio.h>

int raw2gray_init(void) {
    printf("[RAW2GRAY] init\n");
    return 0;
}

int raw2gray(const char *raw_path, const char *gray_path) {
    printf("[RAW2GRAY] converting %s → %s\n", raw_path, gray_path);
    return 0;
}