#include "include/kalloc.h"
#include "memory/heap.h"

void* kalloc(size_t bytes) {
    void* ptr = heap_alloc(bytes);
    return ptr;
}

void kfree(void* ptr) {
    heap_free(ptr);
}