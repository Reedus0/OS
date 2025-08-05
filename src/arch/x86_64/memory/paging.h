#pragma once

#include <stddef.h>
#include <stdint.h>

// Reference: https://wiki.osdev.org/Paging

#include "include/types.h"
#include "memory/discover.h"

struct page_table_entry {
    union {
        uint8_t flags;
        uint64_t address;
        uint8_t present;
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

struct page_table_descriptor {
    byte level;
    short offset;
    page_table_entry_t* table;
};
typedef struct page_table_descriptor page_table_descriptor_t;

physical_page_t g_phisycal_pages[4096];
page_table_descriptor_t* g_page_table_descriptors[512];
size_t g_page_table_descriptors_size = 0;

page_table_entry_t g_kernel_table_l4[512] __attribute__((aligned(4096))) __attribute__((packed));
page_table_entry_t g_kernel_table_l3[512] __attribute__((aligned(4096))) __attribute__((packed));
page_table_entry_t g_kernel_table_l2[512] __attribute__((aligned(4096))) __attribute__((packed));

size_t g_total_pages;
size_t g_available_pages;

size_t allocate_physical_address(size_t virtual_address);
void init_paging();
void map_page(size_t physical_address, size_t virtual_address, size_t flags);
void unmap_page(size_t virtual_address);
void add_physical_pages(memory_chunk_t chunk);