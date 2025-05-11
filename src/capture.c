#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int capture_init(void) {
    // Nothing to init when using libcamera-still
    return 0;
}

int capture_frame_raw(const char *out_path) {
    // Build the libcamera-still command:
    //  - --raw to emit the DNG
    //  - -o to set base output filename (it will add .jpg and .dng)
    //  - --timeout 1 to return immediately after capture
    char cmd[512];
    snprintf(cmd, sizeof(cmd),
             "libcamera-still --raw --timeout 1 -o %s", out_path);

    printf("[CAP] Running: %s\n", cmd);
    int ret = system(cmd);
    if (ret != 0) {
        fprintf(stderr, "[CAP][ERROR] libcamera-still failed (code %d)\n", ret);
        return -1;
    }

    // libcamera-still writes two files:
    //   out_path.jpg  and  out_path.dng
    // We’ll assume the .dng is what we want; the caller can strip “.dng”:
    return 0;
}