#pragma once

#include <stdint.h>
#include <stddef.h>

uint64_t get_page_fault_address();
void flush_page();