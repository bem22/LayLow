// tests/test_png_write.c

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include "../src/image-processing/raw2gray.h"

int main(void) {
    // build a 2×2 GrayImage
    GrayImage g = {0};
    g.width = g.height = 2;
    g.data = malloc(4 * sizeof *g.data);
    g.data[0] = 1000;
    g.data[1] = 2000;
    g.data[2] = 3000;
    g.data[3] = 4000;

    const char *path = "/tmp/test.png";
    int rc = raw2gray_write_png(&g, path);
    assert(rc == 0);

    // now read it back
    FILE *fp = fopen(path, "rb");
    assert(fp);
    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    assert(png);
    png_infop info = png_create_info_struct(png);
    assert(info);
    png_init_io(png, fp);
    png_read_info(png, info);

    png_uint_32 w, h;
    int bit_depth, color_type;
    png_get_IHDR(png, info, &w, &h, &bit_depth, &color_type, NULL, NULL, NULL);
    assert(w == 2 && h == 2);
    assert(bit_depth == 16);
    assert(color_type == PNG_COLOR_TYPE_GRAY);

    // cleanup
    png_destroy_read_struct(&png, &info, NULL);
    fclose(fp);
    free(g.data);

    printf("test_png_write: PASS\n");
    return 0;
}
