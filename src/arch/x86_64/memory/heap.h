#pragma once

#include <stdint.h>
#include <stddef.h>

struct heap_descriptor {
    union {
        uint64_t address;
        uint64_t available;
    };
    uint64_t size;
};
typedef struct heap_descriptor heap_descriptor_t;

size_t g_heap_descriptor_count;

void init_heap();
void* heap_alloc(size_t bytes);