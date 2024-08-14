#include "include/kalloc.h"
#include "arch/x86_64/memory/heap.h"

void* kalloc(size_t bytes) {
    void* ptr = heap_alloc(bytes);
    return ptr;
}

void kfree(void* ptr) {
    heap_free(ptr);
}