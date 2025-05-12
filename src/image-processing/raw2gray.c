// src/image-processing/raw2gray.c

#include "raw2gray.h"
#include <libraw/libraw.h>
#include <png.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Load a RAW file from disk into RawImage
RawImage raw_open(const char *path) {
    RawImage out = {0};
    // libraw_init returns a libraw_data_t*
    libraw_data_t *lr = libraw_init(0);
    if (!lr) {
        fprintf(stderr, "libraw_init failed\n");
        return out;
    }
    if (libraw_open_file(lr, path) != LIBRAW_SUCCESS ||
        libraw_unpack(lr)       != LIBRAW_SUCCESS) {
        fprintf(stderr, "libraw open/unpack failed\n");
        libraw_close(lr);
        return out;
    }
    // Request 16 bits per sample, linear gamma
    lr->params.output_bps = 16;
    lr->params.gamm[0]    = 1.0f;
    lr->params.gamm[1]    = 1.0f;

    lr->params.no_auto_bright  = 0;  // enable dcraw’s auto-bright
    lr->params.use_camera_wb    = 1;  // use camera white balance
    lr->params.use_auto_wb      = 1;  // or let libraw pick WB
    
    if (libraw_dcraw_process(lr) != LIBRAW_SUCCESS) {
        fprintf(stderr, "libraw dcraw process failed\n");
        libraw_close(lr);
        return out;
    }

    // The second argument (errc) can be NULL if you don't need the code
    libraw_processed_image_t *img = libraw_dcraw_make_mem_image(lr, NULL);
    if (!img) {
        fprintf(stderr, "libraw make mem image failed\n");
        libraw_close(lr);
        return out;
    }

    // Fill out the RawImage struct
    out.width  = img->width;
    out.height = img->height;
    size_t npixels = (size_t)out.width * out.height * img->colors;
    out.data   = malloc(sizeof(uint16_t) * npixels);
    if (!out.data) {
        perror("malloc");
    } else {
        // img->data is a pointer to 16-bit samples (RGBRGB…)
        memcpy(out.data, img->data, sizeof(uint16_t) * npixels);
    }

    // Clean up libraw structures
    libraw_dcraw_clear_mem(img);
    libraw_close(lr);
    return out;
}

void raw_free(RawImage *raw) {
    free(raw->data);
    raw->data = NULL;
}


/// Convert a RawImage (RGBRGB…) into a single-channel GrayImage.
#include "raw2gray.h"
#include <libraw/libraw.h>
#include <png.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdint.h>


GrayImage raw2gray(const RawImage *raw) {
    GrayImage g = {0};
    if (!raw->data || raw->width == 0 || raw->height == 0)
        return g;

    size_t w = raw->width, h = raw->height;
    g.width  = w;
    g.height = h;
    g.data   = malloc(w * h * sizeof *g.data);
    if (!g.data) return g;

    const uint16_t *pix = raw->data;
    for (size_t i = 0; i < w*h; i++) {
        float R = pix[3*i + 0];
        float G = pix[3*i + 1];
        float B = pix[3*i + 2];

        // 1) Apply NTSC (Rec.601) formula:
        float Yf = 0.299f * R
                 + 0.587f * G
                 + 0.114f * B;

        // 2) Clamp into [0..65535]
        if (Yf < 0.0f)       Yf = 0.0f;
        else if (Yf > 65535) Yf = 65535.0f;

        // 3) Store as 16-bit integer
        g.data[i] = (uint16_t)(Yf + 0.5f);
    }

    return g;
}

void gray_free(GrayImage *g) {
    free(g->data);
    g->data = NULL;
}

int raw2gray_write_png(const GrayImage *g, const char *png_path) {
    FILE *fp = fopen(png_path, "wb");
    if (!fp) {
        perror("fopen");
        return -1;
    }

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING,
                                               NULL, NULL, NULL);
    if (!png) {
        fclose(fp);
        return -1;
    }

    png_infop info = png_create_info_struct(png);
    if (!info) {
        png_destroy_write_struct(&png, NULL);
        fclose(fp);
        return -1;
    }

    if (setjmp(png_jmpbuf(png))) {
        png_destroy_write_struct(&png, &info);
        fclose(fp);
        return -1;
    }

    png_init_io(png, fp);

    // 1) write the IHDR chunk
    png_set_IHDR(png, info,
                 (png_uint_32)g->width, (png_uint_32)g->height, 16,
                 PNG_COLOR_TYPE_GRAY,
                 PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT,
                 PNG_FILTER_TYPE_DEFAULT);

    // **MISSING STEP**: you must call png_write_info() here
    png_write_info(png, info);

    // 2) write the image data row by row
    for (unsigned y = 0; y < g->height; y++) {
        png_bytep row = (png_bytep)&g->data[y * g->width];
        png_write_row(png, row);
    }

    // 3) finish and clean up
    png_write_end(png, NULL);
    png_destroy_write_struct(&png, &info);
    fclose(fp);
    return 0;
}