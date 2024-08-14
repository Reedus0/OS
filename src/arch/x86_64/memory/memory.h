#pragma once

#include <stdint.h>
#include <stddef.h>

// Reference: https://wiki.osdev.org/Detecting_Memory_(x86)

#include "include/list.h"
#include "include/types.h"
#include "arch/x86_64/boot/multiboot2.h"

#define KERNEL_OFFSET 0x10000000
#define MAX_ADDRESS_SIZE 40

struct memory_chunk {
    byte* start;
    byte* end;
};
typedef struct memory_chunk memory_chunk_t;

void discover_memory(multiboot2_info_t* mbd);