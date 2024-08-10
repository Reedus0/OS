#include <stdio.h>
#include "memory.h"
#include "paging.h"

static void page_table_entry_set_flags(page_table_entry_t* page_table_entry, uint8_t flags) {
    page_table_entry->address |= flags;
}

static void page_table_entry_set_address(page_table_entry_t* page_table_entry, uint64_t address) {
    page_table_entry->address |= address;
}

static void fill_table(size_t base_address, page_table_entry_t* table) {
    for (size_t i = 0; i < TABLE_SIZE; i++) {
        page_table_entry_t* new_page = table + i;
        page_table_entry_set_flags(new_page, 0x83);
        page_table_entry_set_address(new_page, base_address * i);
    }
}

void init_pages() {
    
}