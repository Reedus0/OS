#pragma once

#include <stdint.h>
#include <stddef.h>

uint64_t get_page_fault_address();
void flush_page();
void set_page_directory(page_table_entry_t* address);
uint64_t get_page_directory();