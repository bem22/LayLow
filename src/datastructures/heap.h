#ifndef HEAP_H
#define HEAP_H

#include <stdint.h>

typedef struct {
    uint16_t value;
    int x, y;
} Pixel;

typedef struct {
    Pixel *data;   // array of size cap
    int   size;    // number of elements currently in heap
    int   cap;     // K
} MinHeap;

// Create a heap with capacity K; returns NULL on OOM
MinHeap *heap_create(int K);
// Free the heap
void      heap_destroy(MinHeap *h);

// Offer a new Pixel; keeps only the top K brightest
void      heap_offer(MinHeap *h, Pixel p);

// Pop the smallest (root) element; only valid if h->size > 0
Pixel     heap_pop(MinHeap *h);

#endif // HEAP_H