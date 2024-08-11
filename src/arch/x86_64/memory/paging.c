#include <stdio.h>
#include "memory.h"
#include "paging.h"

static void page_table_entry_set_flags(page_table_entry_t* page_table_entry, uint8_t flags) {
    page_table_entry->address |= flags;
}

static void page_table_entry_set_address(page_table_entry_t* page_table_entry, char* address) {
    page_table_entry->address ^= (page_table_entry->address & ~0xFFF);
    page_table_entry->address |= (uint64_t)address & ~0xFFF;
}

static void page_table_entry_map(page_table_entry_t* page_table_entry) {
    page_table_entry->address |= 1;
}

static void page_table_entry_unmap(page_table_entry_t* page_table_entry) {
    page_table_entry->address ^= page_table_entry->address & 1;
}

static void fill_table() {
    page_table_entry_t* l3_address = g_page_table_l4.address & ~0xFFF;

    for (size_t i = 1; i < TABLE_SIZE; i++) {
        page_table_entry_t* l3_table = l3_address + i;
        page_table_entry_t* l2_table = &g_page_table_pool + i * TABLE_SIZE;

        page_table_entry_set_flags(l3_table, 0x2);
        page_table_entry_set_address(l3_table, l2_table);

        for (size_t j = 0; j < TABLE_SIZE; j++) {
            page_table_entry_t* current_page = l2_table + j;
            page_table_entry_set_flags(current_page, 0x82);
        }
    }
}

page_table_entry_t* get_page_by_index(size_t index, size_t depth) {
    if (index > TABLE_SIZE * TABLE_SIZE) {
        panic("Address out of range");
    }

    page_table_entry_t* prev_address = g_page_table_l4.address & ~0xFFF;
    page_table_entry_t* next_table;

    size_t original_depth = depth;

    while(depth > 0) {
        size_t ln = (index / TABLE_SIZE) % TABLE_SIZE;
        next_table = prev_address + ln;
        prev_address = next_table->address & ~0xFFF;
        index *= TABLE_SIZE;
        depth--;
    }

    return next_table;
}

size_t get_page_index(size_t address) {
    return address / PAGE_SIZE;
}

void map_page(size_t index) {
    page_table_entry_t* l3_table = get_page_by_index(index, 1);
    page_table_entry_t* l2_table = get_page_by_index(index, 2);

    page_table_entry_set_address(l2_table, (MAX_PAGE_COUNT - g_page_count) * PAGE_SIZE);

    page_table_entry_map(l3_table);
    page_table_entry_map(l2_table);

    g_page_count -= 1;
}

void unmap_page(size_t index) {
    page_table_entry_t* l3_table = get_page_by_index(index, 1);
    page_table_entry_t* l2_table = get_page_by_index(index, 2);

    page_table_entry_set_address(l2_table, 0);

    page_table_entry_unmap(l3_table);
    page_table_entry_unmap(l2_table);

    g_page_count += 1;
}

void init_pages() {
    fill_table();
}