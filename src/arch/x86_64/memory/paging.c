#include <stdio.h>
#include "memory.h"
#include "paging.h"
#include "arch/x86_64/asm/paging.h"

static void page_table_entry_set_flags(page_table_entry_t* page_table_entry, uint8_t flags) {
    page_table_entry->flags |= flags;
}

static void page_table_entry_set_address(page_table_entry_t* page_table_entry, char* address) {
    page_table_entry->address ^= (page_table_entry->address & ~0x3FF);
    page_table_entry->address |= (uint64_t)address & ~0x3FF;
}

static void page_table_entry_map(page_table_entry_t* page_table_entry) {
    page_table_entry->address |= 1;
}

static void page_table_entry_unmap(page_table_entry_t* page_table_entry) {
    page_table_entry->address ^= page_table_entry->address & 1;
}

void map_page(size_t physical_address, size_t virtual_address, size_t flags) {
    size_t masked_address = virtual_address;

    size_t l4_offset = (masked_address >> 39);
    size_t l3_offset = (masked_address >> 30);
    size_t l2_offset = (masked_address >> 21);

    page_table_entry_t* l4_address = &g_page_table_l4 + l4_offset;
    page_table_entry_t* l3_address = &g_page_table_l3 + l3_offset;
    page_table_entry_t* l2_address = &g_page_table_l2 + l2_offset;

    page_table_entry_set_address(l3_address, l2_address);
    page_table_entry_set_address(l2_address, physical_address);

    page_table_entry_set_flags(l3_address, 0x2);
    page_table_entry_set_flags(l2_address, flags);

    page_table_entry_map(l3_address);
    page_table_entry_map(l2_address);
}

void unmap_page(size_t virtual_address) {
    size_t masked_address = virtual_address;

    size_t l4_offset = (masked_address >> 39);
    size_t l3_offset = (masked_address >> 30);
    size_t l2_offset = (masked_address >> 21);

    page_table_entry_t* l4_address = &g_page_table_l4 + l4_offset;
    page_table_entry_t* l3_address = &g_page_table_l3 + l3_offset;
    page_table_entry_t* l2_address = &g_page_table_l2 + l2_offset;

    page_table_entry_set_address(l3_address, 0);
    page_table_entry_set_address(l2_address, 0);

    page_table_entry_set_flags(l3_address, 0);
    page_table_entry_set_flags(l2_address, 0);

    page_table_entry_unmap(l3_address);
    page_table_entry_unmap(l2_address);
}