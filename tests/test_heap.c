#include <assert.h>
#include <stdio.h>
#include "../src/datastructures/heap.h"

int main(void) {
    // Create a min-heap of capacity 4
    MinHeap *h = heap_create(4);
    assert(h);

    // Offer 6 pixels; only top-4 by value should remain
    Pixel inputs[6] = {
        {10,0,0},{30,0,1},{20,1,0},
        {40,1,1},{50,2,0},{25,2,1}
    };
    for (int i = 0; i < 6; i++) {
        heap_offer(h, inputs[i]);
    }
    // Heap should hold the 4 largest values: 20,25,30,40,50 -> keep {30,40,50,25}? Actually top-4 exclude 10,20
    assert(h->size == 4);

    // Pop all into array: should come out ascending among kept: e.g. 25,30,40,50
    Pixel popped[4];
    for (int i = h->size-1; i >= 0; i--) {
        popped[i] = heap_pop(h);
    }
    int expected[4] = {50,40,30,25};
    for (int i = 0; i < 4; i++) {
        assert(popped[i].value == expected[i]);
    }

    printf("test_heap: PASS\n");
    heap_destroy(h);
    return 0;
}
