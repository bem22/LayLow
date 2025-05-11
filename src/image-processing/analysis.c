#include "../common/common.h"
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

#include "analysis.h"
#include <stdio.h>
#include <stdlib.h>

// Finds the N brightest pixels in gray and returns their coords+values in descending order.
AnalysisResult find_brightest(const GrayImage *g, size_t N) {
    AnalysisResult r = {0};

    // clamp N to image size
    size_t total = g->width * g->height;
    if (N == 0 || total == 0) return r;
    if (N > total) N = total;

    // allocate temporary Pixel buffer
    Pixel *px = malloc(N * sizeof *px);
    if (!px) return r;

    // run heap-based top-K
    if (find_top_k(g->data, (int)g->width, (int)g->height, (int)N, px) != 0) {
        free(px);
        return r;
    }

    // allocate result arrays
    r.xs     = malloc(N * sizeof *r.xs);
    r.ys     = malloc(N * sizeof *r.ys);
    r.values = malloc(N * sizeof *r.values);
    if (!r.xs || !r.ys || !r.values) {
        free(px);
        free(r.xs); free(r.ys); free(r.values);
        return r;
    }

    // unpack pixels into result
    r.count = N;
    for (size_t i = 0; i < N; i++) {
        r.xs[i]     = (unsigned)px[i].x;
        r.ys[i]     = (unsigned)px[i].y;
        r.values[i] = px[i].value;
    }

    free(px);
    return r;
}

int analysis_write_report(const AnalysisResult *r, const char *path) {
    FILE *f = fopen(path, "w");
    if (!f) return -1;
    for (size_t i = 0; i < r->count; i++) {
        fprintf(f, "%u %u %u\n",
                r->xs[i], r->ys[i], r->values[i]);
    }
    fclose(f);
    return 0;
}

void analysis_free(AnalysisResult *r) {
    free(r->xs);
    free(r->ys);
    free(r->values);
}
