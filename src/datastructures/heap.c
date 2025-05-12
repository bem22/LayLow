#include "heap.h"
#include <stdlib.h>

static void swap(Pixel *a, Pixel *b) {
    Pixel t = *a; *a = *b; *b = t;
}

static void sift_up(MinHeap *h, int idx) {
    while (idx > 0) {
        int parent = (idx - 1) / 2;
        if (h->data[idx].value < h->data[parent].value) {
            swap(&h->data[idx], &h->data[parent]);
            idx = parent;
        } else break;
    }
}

static void sift_down(MinHeap *h, int idx) {
    int n = h->size;
    while (1) {
        int l = 2*idx + 1, r = l + 1, smallest = idx;
        if (l < n && h->data[l].value < h->data[smallest].value)
            smallest = l;
        if (r < n && h->data[r].value < h->data[smallest].value)
            smallest = r;
        if (smallest == idx) break;
        swap(&h->data[idx], &h->data[smallest]);
        idx = smallest;
    }
}

MinHeap *heap_create(int K) {
    MinHeap *h = malloc(sizeof *h);
    if (!h) return NULL;
    h->data = malloc(K * sizeof *h->data);
    if (!h->data) { free(h); return NULL; }
    h->size = 0;
    h->cap  = K;
    return h;
}

void heap_destroy(MinHeap *h) {
    if (!h) return;
    free(h->data);
    free(h);
}

void heap_offer(MinHeap *h, Pixel p) {
    if (h->size < h->cap) {
        h->data[h->size] = p;
        sift_up(h, h->size);
        h->size++;
    } else if (p.value > h->data[0].value) {
        h->data[0] = p;
        sift_down(h, 0);
    }
}

Pixel heap_pop(MinHeap *h) {
    Pixel root = h->data[0];
    h->size--;
    h->data[0] = h->data[h->size];
    sift_down(h, 0);
    return root;
}