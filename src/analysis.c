#include "common.h"
#include "analysis.h"
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

// Finds the K brightest pixels in `gray[0..w*h-1]`
// and writes them in descending order into out[0..K-1].
int find_top_k(uint16_t *gray, int w, int h, int K, Pixel out[]) {
    MinHeap *hpr = heap_create(K);
    if (!hpr) return -1;

    for (int y = 0; y < h; y++) {
      for (int x = 0; x < w; x++) {
        Pixel p = { gray[y*w + x], x, y };
        heap_offer(hpr, p);
      }
    }

    // Extract in ascending order, store backwards for descending
    for (int i = hpr->size - 1; i >= 0; i--) {
      out[i] = heap_pop(hpr);
    }
    heap_destroy(hpr);
    return 0;
}