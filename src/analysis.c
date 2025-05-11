#include "common.h"
#include <stdio.h>
#include <stdint.h>

int analysis_init(void) {
    printf("[ANALYSIS] init\n");
    return 0;
}

int analysis_threshold(const char *gray_path, const char *bin_path, uint16_t threshold) {
    printf("[ANALYSIS] threshold %s → %s @ %u\n", gray_path, bin_path, threshold);
    return 0;
}

int analysis_edge(const char *gray_path, const char *edge_path, int low, int high) {
    printf("[ANALYSIS] edge-detect %s → %s [%d,%d]\n", gray_path, edge_path, low, high);
    return 0;
}