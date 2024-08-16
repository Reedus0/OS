#pragma once

#include <stddef.h>
#include <stdint.h>

// Reference: https://wiki.osdev.org/Paging

#include "arch/x86_64/memory/discover.h"

#define PAGE_SIZE 0x200000
#define KERNEL_OFFSET 0x10000000

struct page_table_entry {
    union {
        uint8_t flags;
        uint64_t address;
    };
} __attribute__((packed));
typedef struct page_table_entry page_table_entry_t;

struct physical_page {
    union {
        uint64_t address;
        uint64_t available;
    };
    uint64_t* virtual_address;
};
typedef struct physical_page physical_page_t;

size_t g_total_pages;
size_t g_available_pages;

size_t allocate_physical_address(size_t virtual_address);
void map_page(size_t physical_address, size_t virtual_address, size_t flags);
void unmap_page(size_t virtual_address);
void add_physical_pages(memory_chunk_t chunk);
void init_pages();