#pragma once

#include <stdint.h>
#include <stddef.h>

/// Holds a packed RAW image (unpacked Bayer, etc).
typedef struct {
    uint16_t *data;    // raw pixel array
    size_t    width, height;
} RawImage;

/// A 16-bit grayscale buffer.
typedef struct {
    uint16_t *data;
    size_t    width, height;
} GrayImage;

/// Load a RAW file from disk.
RawImage raw_open(const char *path);

/// Free RawImage data.
void raw_free(RawImage *raw);

/// Convert to 16-bit grayscale using Rec.601 luma.
GrayImage raw2gray(const RawImage *raw);

/// Free GrayImage data.
void gray_free(GrayImage *g);

/// Write a GrayImage out as a 16-bit PNG.
int raw2gray_write_png(const GrayImage *g, const char *png_path);

