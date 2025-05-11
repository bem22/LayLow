#include "common.h"
#include <libraw/libraw.h>
#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Helper to write a 16-bit-per-pixel grayscale PNG
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
    // 16-bit, grayscale
    png_set_IHDR(png, info, width, height,
                 16, PNG_COLOR_TYPE_GRAY,
                 PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT,
                 PNG_FILTER_TYPE_DEFAULT);
    png_write_info(png, info);

    for (int y = 0; y < height; y++) {
        // each row = width samples of 16 bits = width * sizeof(uint16_t) bytes
        png_write_row(png, (png_bytep)(buf + (size_t)y * width));
    }

    png_write_end(png, NULL);
    png_destroy_write_struct(&png, &info);
    fclose(fp);
    return 0;
}

int raw2gray_init(void) {
    // nothing needed here
    return 0;
}

int raw2gray(const char *raw_path, const char *gray_path) {
    LibRaw processor;
    int ret = processor.open_file(raw_path);
    if (ret != LIBRAW_SUCCESS) {
        fprintf(stderr, "RAW2GRAY: cannot open %s: %s\n",
                raw_path, libraw_strerror(ret));
        return -1;
    }

    ret = processor.unpack();
    if (ret != LIBRAW_SUCCESS) {
        fprintf(stderr, "RAW2GRAY: unpack error: %s\n",
                libraw_strerror(ret));
        processor.recycle();
        return -1;
    }

    int w = processor.imgdata.sizes.width;
    int h = processor.imgdata.sizes.height;
    uint16_t *raw = processor.imgdata.image;  // interleaved Bayer

    // Allocate output buffer (one sample per pixel)
    uint16_t *gray = malloc((size_t)w * h * sizeof *gray);
    if (!gray) {
        perror("malloc");
        processor.recycle();
        return -1;
    }

    // Simple: take the first channel (e.g. R in an RGBG pattern)
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            gray[y*w + x] = raw[(y * w + x) * 3];
        }
    }

    ret = write_png_16(gray_path, gray, w, h);
    free(gray);
    processor.recycle();

    if (ret == 0)
        printf("[RAW2GRAY] Wrote 16-bit grayscale: %s\n", gray_path);
    return ret;
}