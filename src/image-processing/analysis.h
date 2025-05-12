#ifndef ANALYSIS_H
#define ANALYSIS_H

#include "../common/common.h"

#include "../image-processing/raw2gray.h"    // for GrayImage
#include <stdint.h>

// Analysis function declarations
int analysis_init(void);
int analysis_threshold(const char *gray_path, const char *bin_path, uint16_t threshold);
int analysis_edge(const char *gray_path, const char *edge_path, int low, int high);
int find_top_k(uint16_t *gray, int w, int h, int K, Pixel out[]);

// Helper function declarations with proper C syntax
void heap_insert(Pixel heap[], int *size, Pixel p);
void heap_sift_up(Pixel heap[], int idx);
void heap_sift_down(Pixel heap[], int size, int idx);
Pixel heap_pop_min(Pixel heap[], int *size);


typedef struct {
    // you might store coords + values arrays, or just summaries
    size_t count;
    unsigned *xs, *ys;
    uint16_t *values;
} AnalysisResult;

/// Find the N brightest pixels in a GrayImage.
AnalysisResult find_brightest(const GrayImage *g, size_t N);

/// Write out a human-readable report:
/// each line “x y value” or similar.
int analysis_write_report(const AnalysisResult *r, const char *path);

/// Free any malloc’ed fields in AnalysisResult.
void analysis_free(AnalysisResult *r);


#endif // ANALYSIS_H
