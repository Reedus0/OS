#pragma once

#include <stdint.h>
#include <stddef.h>

#include "memory/discover.h"

extern physical_page_t g_phisycal_pages[];

extern size_t g_page_table_l4;
extern size_t g_page_table_l3;
extern size_t g_page_table_l2;

uint64_t get_page_fault_address();
void flush_page();