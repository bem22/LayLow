#include "common.h"
#include <stdio.h>

int main(int argc, char **argv) {
    capture_init();
    raw2gray_init();
    analysis_init();
    bt_init();

    printf("All modules initialized\n");

    // Example flow:
    capture_frame_raw("data/test.dng");
    raw2gray("data/test.dng", "data/test.tiff");
    analysis_threshold("data/test.tiff", "data/test_bin.png", 1000);
    analysis_edge("data/test.tiff", "data/test_edge.png", 50, 150);

    bt_connect("01:23:45:67:89:AB");
    bt_send_cmd("CAPTURE");
    char status[64];
    bt_receive_status(status, sizeof(status));
    printf("Remote status: %s\n", status);
    int batt;
    bt_read_battery(&batt);
    printf("Remote battery: %d%%\n", batt);

    return 0;
}