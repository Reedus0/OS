#pragma once

#include <stdint.h>
#include <stddef.h>

extern physical_page_t g_phisycal_pages[4096];

extern page_table_entry_t g_page_table_l4[512];
extern page_table_entry_t g_page_table_l3[512];
extern page_table_entry_t g_page_table_l2[512 * 512];

uint64_t get_page_fault_address();
void flush_page();