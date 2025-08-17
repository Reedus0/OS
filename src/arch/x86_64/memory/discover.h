#pragma once

#include <stdint.h>
#include <stddef.h>

// Reference: https://wiki.osdev.org/Detecting_Memory_(x86)

#include "include/types.h"
#include "boot/multiboot2.h"

struct memory_chunk {
    uint64_t start;
    uint64_t end;
};
typedef struct memory_chunk memory_chunk_t;

void discover_memory(multiboot2_info_t* mbd);