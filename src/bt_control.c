#include "common.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int bt_init(void) {
    printf("[BT] init\n");
    return 0;
}

int bt_connect(const char *mac) {
    printf("[BT] connect to %s\n", mac);
    return 0;
}

int bt_send_cmd(const char *cmd) {
    printf("[BT] send cmd: %s\n", cmd);
    return 0;
}

int bt_receive_status(char *buf, size_t buflen) {
    snprintf(buf, buflen, "OK");   // stub
    sleep(1);
    return 0;
}

int bt_read_battery(int *percent) {
    *percent = 42;                  // stub
    printf("[BT] battery: %d%%\n", *percent);
    return 0;
}
