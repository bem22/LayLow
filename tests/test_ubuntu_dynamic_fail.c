#include <assert.h>
#include <stdio.h>
#include <stdlib.h>           // for system()
#include "../src/camera/capture.h"

int main(void) {
    // Only run if libcamera-still is absent
    if (system("which libcamera-still >/dev/null 2>&1") == 0) {
        printf("test_ubuntu_dynamic_fail: SKIPPED (libcamera-still present)\n");
        return 0;
    }

    int rc = capture_to_raw("/tmp/ubuntu_should_fail.dng");
    // Expect non-zero return on Ubuntu
    assert(rc != 0);

    printf("test_ubuntu_dynamic_fail: PASS (capture_to_raw correctly failed)\n");
    return 0;
}
