#include "common.h"
#include <libraw/libraw_c_api.h>
#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Write 16-bit grayscale PNG
static int write_png_16(const char *fn, uint16_t *buf, int w, int h) {
    FILE *fp = fopen(fn, "wb");
    if (!fp) { perror("fopen"); return -1; }
    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING,
                                               NULL, NULL, NULL);
    if (!png) { fclose(fp); return -1; }
    png_infop info = png_create_info_struct(png);
    if (!info) { png_destroy_write_struct(&png, NULL);
                 fclose(fp); return -1; }
    if (setjmp(png_jmpbuf(png))) {
        png_destroy_write_struct(&png, &info);
        fclose(fp);
        return -1;
    }
    png_init_io(png, fp);
    png_set_IHDR(png, info, w, h, 16, PNG_COLOR_TYPE_GRAY,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
                 PNG_FILTER_TYPE_DEFAULT);
    png_write_info(png, info);
    for (int y = 0; y < h; y++)
        png_write_row(png, (png_bytep)(buf + (size_t)y * w));
    png_write_end(png, NULL);
    png_destroy_write_struct(&png, &info);
    fclose(fp);
    return 0;
}

int raw2gray_init(void) {
    return 0;
}

int raw2gray(const char *raw_path, const char *gray_path) {
    libraw_data_t *proc = libraw_init(0);
    if (!proc) {
        fprintf(stderr, "RAW2GRAY: init failed\n");
        return -1;
    }
    int rc = libraw_open_file(proc, raw_path);
    if (rc != LIBRAW_SUCCESS) {
        fprintf(stderr, "RAW2GRAY: open %s failed: %s\n",
                raw_path, libraw_strerror(rc));
        libraw_close(proc);
        return -1;
    }
    if ((rc = libraw_unpack(proc)) != LIBRAW_SUCCESS) {
        fprintf(stderr, "RAW2GRAY: unpack error: %s\n",
                libraw_strerror(rc));
        libraw_close(proc);
        return -1;
    }

    int w = proc->sizes.width;
    int h = proc->sizes.height;
    uint16_t *raw = proc->rawdata.raw_image;
    uint16_t *gray = malloc((size_t)w * h * sizeof *gray);
    if (!gray) { perror("malloc"); libraw_close(proc); return -1; }

    // Extract one channel—here, every 3rd sample
    for (int i = 0; i < w*h; i++)
        gray[i] = raw[3*i];

    rc = write_png_16(gray_path, gray, w, h);
    free(gray);
    libraw_close(proc);

    if (rc == 0)
        printf("[RAW2GRAY] wrote %s\n", gray_path);
    return rc;
}