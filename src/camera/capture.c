#include "capture.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

RawImage capture_from_camera(const char *path) {
    RawImage raw = {0};
    if (capture_to_raw(path) < 0) {
        fprintf(stderr, "capture_to_raw failed\n");
        return raw;
    }
    // now load it exactly like a static file
    raw = raw_open(path);
    return raw;
}

int capture_to_raw(const char *path) {
    // 1) pick the available libcamera CLI tool
    const char *cmd_template = NULL;
    if (system("which libcamera-still >/dev/null 2>&1") == 0) {
        // libcamera-still: raw+JPEG output
        cmd_template = "libcamera-still --nopreview --raw -t 1000 -o %s";
    }
    else if (system("which libcamera-raw >/dev/null 2>&1") == 0) {
        // libcamera-raw: raw output only
        cmd_template = "libcamera-raw -o %s";
    }
    else {
        fprintf(stderr,
                "capture_to_raw error: neither libcamera-still nor libcamera-raw\n"
                "  is installed. Please install libcamera-apps or similar.\n");
        return -1;
    }

    // 2) build the final command
    char cmd[1024];
    int n = snprintf(cmd, sizeof(cmd), cmd_template, path);
    if (n < 0 || (size_t)n >= sizeof(cmd)) {
        fprintf(stderr, "capture_to_raw error: command buffer overflow\n");
        return -1;
    }

    // 3) invoke it
    int ret = system(cmd);
    if (ret != 0) {
        fprintf(stderr, "capture_to_raw: system(\"%s\") returned %d\n",
                cmd, ret);
        return -1;
    }
    return 0;
}
