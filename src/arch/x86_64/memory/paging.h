#pragma once

#include <stddef.h>
#include <stdint.h>

// Reference: https://wiki.osdev.org/Paging

#define PAGE_SIZE 0x200000
#define KERNEL_OFFSET 0xC0000000

union page_table_entry {
    uint8_t flags;
    uint64_t address;
} __attribute__((packed));
typedef union page_table_entry page_table_entry_t;

extern page_table_entry_t g_page_table_l4;
extern page_table_entry_t g_page_table_l3;
extern page_table_entry_t g_page_table_l2;

void map_page(size_t physical_address, size_t virtual_address, size_t flags);
void unmap_page(size_t virtual_address);
void init_pages();