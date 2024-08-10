#pragma once

#include <stddef.h>
#include <stdint.h>

// Reference: https://wiki.osdev.org/Paging

#define PAGE_SIZE 0x400000
#define TABLE_SIZE 1024

struct page_table_entry {
    uint64_t address;
} __attribute__((packed));
typedef struct page_table_entry page_table_entry_t;

extern page_table_entry_t g_page_table_l4[512];
extern page_table_entry_t g_page_table_l3[512];
extern page_table_entry_t g_page_table_l2[512];

void init_pages();