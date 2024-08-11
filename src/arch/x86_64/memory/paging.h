#pragma once

#include <stddef.h>
#include <stdint.h>

// Reference: https://wiki.osdev.org/Paging

#define PAGE_SIZE 0x200000
#define TABLE_SIZE 512
#define MAX_PAGE_COUNT 262143

struct page_table_entry {
    uint64_t address;
} __attribute__((packed));
typedef struct page_table_entry page_table_entry_t;

extern page_table_entry_t g_page_table_l4; // Page l4 onle have 1 entry because we can address
extern page_table_entry_t g_page_table_l3; // As much as 35 TB of memory with only one entry
extern page_table_entry_t g_page_table_l2;
extern page_table_entry_t g_page_table_pool;

size_t g_page_count = 512 * 512 - 512 - 1;

page_table_entry_t* get_page_by_index(size_t index, size_t depth);
page_table_entry_t* get_page_by_address(size_t address, size_t depth);
size_t get_page_index(size_t address);
void init_pages();