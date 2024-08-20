#pragma once

#include <stddef.h>
#include <stdint.h>

// Reference: https://wiki.osdev.org/Paging

#include "memory/discover.h"

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

extern physical_page_t g_phisycal_pages[4096];

extern page_table_entry_t g_page_table_l4[512];
extern page_table_entry_t g_page_table_l3[512];
extern page_table_entry_t g_page_table_l2[512 * 512];

// physical_page_t g_phisycal_pages[PHYSICAL_PAGE_COUNT];

// byte g_page_table_l4[0x1000] __attribute__((aligned (4096)));
// byte g_page_table_l3[0x1000] __attribute__((aligned (4096)));
// byte g_page_table_l2[0x1000 * 512] __attribute__((aligned (4096)));


size_t g_total_pages;
size_t g_available_pages;

size_t allocate_physical_address(size_t virtual_address);
void map_page(size_t physical_address, size_t virtual_address, size_t flags);
void unmap_page(size_t virtual_address);
void add_physical_pages(memory_chunk_t chunk);
void init_pages();