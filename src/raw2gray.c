#include "common.h"
#include <libraw/libraw_c_api.h>
#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Helper to write a 16-bit PNG (same as before)
static int write_png_16(const char *filename, uint16_t *buf, int width, int height) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) { perror("fopen"); return -1; }

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) { fclose(fp); return -1; }

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
    png_set_IHDR(png, info, width, height, 16, PNG_COLOR_TYPE_GRAY,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
                 PNG_FILTER_TYPE_DEFAULT);
    png_write_info(png, info);

    for (int y = 0; y < height; y++) {
        png_write_row(png, (png_bytep)(buf + (size_t)y * width));
    }

    png_write_end(png, NULL);
    png_destroy_write_struct(&png, &info);
    fclose(fp);
    return 0;
}

int raw2gray_init(void) {
    return 0;
}

int raw2gray(const char *raw_path, const char *gray_path) {
    // Initialize
    libraw_data_t *processor = libraw_init(0);
    if (!processor) {
        fprintf(stderr, "RAW2GRAY: libraw_init failed\n");
        return -1;
    }

    // Open file
    int rc = libraw_open_file(processor, raw_path);
    if (rc != LIBRAW_SUCCESS) {
        fprintf(stderr, "RAW2GRAY: cannot open %s: %s\n",
                raw_path, libraw_strerror(rc));
        libraw_close(processor);
        return -1;
    }

    // Unpack
    rc = libraw_unpack(processor);
    if (rc != LIBRAW_SUCCESS) {
        fprintf(stderr, "RAW2GRAY: unpack error: %s\n", libraw_strerror(rc));
        libraw_close(processor);
        return -1;
    }

    // Dimensions & buffer
    int w = processor->sizes.width;
    int h = processor->sizes.height;
    uint16_t *raw = processor->rawdata.raw_image;  // 16-bit samples

    // Allocate grayscale buffer
    uint16_t *gray = malloc((size_t)w * h * sizeof *gray);
    if (!gray) {
        perror("malloc");
        libraw_close(processor);
        return -1;
    }

    // Simple channel extraction (first channel of each pixel)
    for (int i = 0; i < w*h; i++) {
        gray[i] = raw[3*i];  
    }

    // Write PNG
    rc = write_png_16(gray_path, gray, w, h);
    free(gray);
    libraw_close(processor);

    if (rc == 0)
        printf("[RAW2GRAY] Wrote 16-bit grayscale: %s\n", gray_path);
    return rc;
}