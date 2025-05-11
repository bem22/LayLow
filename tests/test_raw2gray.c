#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "../src/image-processing/raw2gray.h"

// Build a 2×2 RawImage with known RGB values
static RawImage make_test_raw(void) {
    RawImage raw = {0};
    raw.width  = 2;
    raw.height = 2;
    raw.data   = malloc(2*2*3 * sizeof *raw.data);
    // Layout (row-major, three channels):
    // [ (0,0,0),        (65535,65535,65535),
    //   (65535,0,0),    (0,65535,0) ]
    uint16_t tmp[] = {
        0,0,0,  65535,65535,65535,
        65535,0,0,  0,65535,0
    };
    for (int i = 0; i < 12; i++) raw.data[i] = tmp[i];
    return raw;
}

int main(void) {
    RawImage raw = make_test_raw();
    GrayImage g = raw2gray(&raw);

    // Should be 2×2
    assert(g.width  == 2);
    assert(g.height == 2);

    // NTSC weights:
    // pixel[0]: (0,0,0)       → Y=0
    // pixel[1]: (65535,65535,65535) → Y≈65535
    // pixel[2]: (65535,0,0)   → Y≈0.299*65535 ≈ 19595
    // pixel[3]: (0,65535,0)   → Y≈0.587*65535 ≈ 38469

    assert(g.data[0] == 0);
    assert(g.data[1] == 65535);

    int v2 = g.data[2];
    int v3 = g.data[3];
    // allow ±2 tolerance
    assert(abs(v2 - 19595) <= 2);
    assert(abs(v3 - 38469) <= 2);

    printf("test_raw2gray: PASS\n");

    gray_free(&g);
    raw_free(&raw);
    return 0;
}
