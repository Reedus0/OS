#pragma once

#include <stdint.h>
#include <stddef.h>

#include "include/types.h"
#include "memory/memory.h"

#define HEAP_DESCRIPTORS_SIZE 0x20000
#define HEAP_CANARY_SIZE 8

struct heap_descriptor {
    uint64_t address;
    uint64_t page_address;
    uint8_t available;
    uint64_t size;
};
typedef struct heap_descriptor heap_descriptor_t;

heap_descriptor_t g_heap_descriptors[HEAP_DESCRIPTORS_SIZE];

size_t g_heap_descriptor_count;

void* heap_alloc(size_t bytes);
void* heap_alloc_aligned(size_t bytes, size_t alignment);