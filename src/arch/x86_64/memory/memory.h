#pragma once

// Reference: https://wiki.osdev.org/Detecting_Memory_(x86)

#include <stdint.h>
#include <stddef.h>

#include "include/list.h"
#include "arch/x86_64/boot/multiboot2.h"

#define KERNEL_OFFSET 0x100000

struct memory_chunk {
    char* start;
    char* end;
};
typedef struct memory_chunk memory_chunk_t;

size_t g_available_memory;
size_t g_memory_chunk_count;
memory_chunk_t g_memory_list[16];

void discover_memory(multiboot2_info_t* mbd);