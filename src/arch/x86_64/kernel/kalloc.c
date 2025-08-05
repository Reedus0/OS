#include "include/kalloc.h"
#include "memory/heap.h"

void* kalloc(size_t bytes) {
    void* ptr = heap_alloc(bytes);
    return ptr;
}

void* kalloc_aligned(size_t bytes, size_t alignment) {
    void* ptr = heap_alloc_aligned(bytes, alignment);
    return ptr;
}

void kfree(void* ptr) {
    heap_free(ptr);
}