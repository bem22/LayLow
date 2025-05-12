#ifndef ANALYSIS_H
#define ANALYSIS_H

#include "common.h"

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

#endif // ANALYSIS_H

typedef struct Pixel {
    uint16_t brightness;
    int pos_x, pos_y;
} Pixel;

void heap_insert(Pixel heap[], int *size, Pixel p)

void heap_sift_up(Pixel heap[], int idx)

void heap_sift_down(Pixel heap[], int size, int idx)

Pixel heap_pop_min(Pixel heap[], int *size)