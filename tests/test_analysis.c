#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "../src/image-processing/analysis.h"

// Build a 3×3 gray image where pixel value = x + y*3
static GrayImage make_test_gray(void) {
    GrayImage g = {0};
    g.width = g.height = 3;
    g.data = malloc(9 * sizeof *g.data);
    for (int y = 0; y < 3; y++)
      for (int x = 0; x < 3; x++)
        g.data[y*3 + x] = (uint16_t)(x + y*3);
    return g;
}

int main(void) {
    GrayImage g = make_test_gray();

    // Find top 4 brightest pixels
    AnalysisResult r = find_brightest(&g, 4);
    assert(r.count == 4);

    // The 4 largest values in [0..8] are 8,7,6,5 at coords:
    // 8: (2,2), 7:(1,2), 6:(0,2), 5:(2,1)
    unsigned exp_x[4] = {2,1,0,2};
    unsigned exp_y[4] = {2,2,2,1};
    uint16_t exp_v[4] = {8,7,6,5};

    for (size_t i = 0; i < 4; i++) {
        assert(r.xs[i]    == exp_x[i]);
        assert(r.ys[i]    == exp_y[i]);
        assert(r.values[i]== exp_v[i]);
    }

    analysis_free(&r);
    gray_free(&g);
    printf("test_analysis: PASS\n");
    return 0;
}
