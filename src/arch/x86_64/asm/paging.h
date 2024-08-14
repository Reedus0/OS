#pragma once

#include <stdint.h>
#include <stddef.h>

extern size_t g_page_table_l4;
extern size_t g_page_table_l3;
extern size_t g_page_table_l2;

uint64_t get_page_fault_address();
void flush_page();