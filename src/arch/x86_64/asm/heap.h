#pragma once

#include <stdint.h>
#include <stddef.h>

#include "include/types.h"
#include "arch/x86_64/memory/heap.h"

#define HEAP_SIZE 0x2000000
#define HEAP_DESCRIPTORS_SIZE 0x200000

extern heap_descriptor_t g_heap_descriptors[];
extern byte g_heap_base[];