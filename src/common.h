
#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <stddef.h>
#include "datastructures/heap.h"

// libcamera capture
int capture_init(void);
int capture_frame_raw(const char *out_path);

// raw -> 16-bit grayscale
int raw2gray_init(void);
int raw2gray(const char *raw_path, const char *gray_path);

// image analysis
int analysis_init(void);
int analysis_threshold(const char *gray_path, const char *bin_path, uint16_t threshold);
int analysis_edge(const char *gray_path, const char *edge_path, int low, int high);
int find_top_k(uint16_t *gray, int w, int h, int K, Pixel out[]);

#endif // COMMON_H