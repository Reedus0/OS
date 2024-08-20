#pragma once

#include <stdint.h>
#include <stddef.h>

#include "include/types.h"

#define HEAP_SIZE 0x2000000
#define HEAP_DESCRIPTORS_SIZE 0x20000

struct heap_descriptor {
    union {
        uint64_t address;
        uint64_t available;
    };
    uint64_t size;
};
typedef struct heap_descriptor heap_descriptor_t;

heap_descriptor_t g_heap_descriptors[HEAP_DESCRIPTORS_SIZE];
byte g_heap_base[HEAP_SIZE];

size_t g_heap_descriptor_count;

void init_heap();
void* heap_alloc(size_t bytes);