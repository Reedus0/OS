#include <stdio.h>
#include "memory.h"
#include "paging.h"

static void page_table_entry_set_flags(page_table_entry_t* page_table_entry, uint8_t flags) {
    page_table_entry->address |= flags;
}

static void page_table_entry_set_address(page_table_entry_t* page_table_entry, char* address) {
    page_table_entry->address |= (uint64_t)address & ~0xFFF;
}

static void fill_table() {
    page_table_entry_t* l4_address = g_page_table_l4.address & ~0xFFF;

    for (size_t i = 1; i < TABLE_SIZE; i++) {
        page_table_entry_t* l3_table = l4_address + i;
        page_table_entry_t* l2_table = &g_page_table_pool + i * TABLE_SIZE;

        page_table_entry_set_flags(l3_table, 0x2);
        page_table_entry_set_address(l3_table, l2_table);

        for (size_t j = 0; j < TABLE_SIZE; j++) {
            page_table_entry_t* current_page = l2_table + j;
            page_table_entry_set_flags(current_page, 0x82);
            page_table_entry_set_address(current_page, PAGE_SIZE * j + TABLE_SIZE * PAGE_SIZE * i);
        }
    }
}

static page_table_entry_t* get_page(size_t index) {
    if (index > TABLE_SIZE * TABLE_SIZE) {
        return NULL;
    }
    size_t l2 = index % TABLE_SIZE;
    size_t l3 = (index / TABLE_SIZE) % TABLE_SIZE;
    size_t l4 = (index / (TABLE_SIZE * TABLE_SIZE)) % TABLE_SIZE;

    page_table_entry_t* l4_address = g_page_table_l4.address & ~0xFFF;

    page_table_entry_t l3_table = *(l4_address + l4);
    page_table_entry_t* l3_address = l3_table.address & ~0xFFF;

    page_table_entry_t* page_address = l3_address + l3 * TABLE_SIZE + l2;

    return page_address;
}

void init_pages() {
    fill_table();
    page_table_entry_t* page = get_page(512 * 512 - 1);

    printf("%x", *page);
}