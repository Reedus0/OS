#pragma once

#include <stdint.h>
#include <stddef.h>

#include "include/types.h"
#include "memory/memory.h"

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
byte* g_heap_base = KERNEL_PHYSICAL_ADDRESS_OFFSET;

size_t g_heap_descriptor_count;

void init_heap();
void* heap_alloc(size_t bytes);
void* heap_alloc_aligned(size_t bytes, size_t alignment);