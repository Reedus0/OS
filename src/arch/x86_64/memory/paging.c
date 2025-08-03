#include "memory.h"
#include "paging.h"
#include "asm/paging.h"

static void physical_page_set_address(physical_page_t* physical_page, uint64_t address) {
    physical_page->address = address;
}

static void physical_page_set_available(physical_page_t* physical_page) {
    physical_page->available |= ((size_t)1 << MAX_ADDRESS_SIZE);
}

static void physical_page_clear_available(physical_page_t* physical_page) {
    physical_page->available &= physical_page->available ^ (size_t)1 << MAX_ADDRESS_SIZE;
}

static char physical_page_is_available(physical_page_t* physical_page) {
    return physical_page->available >> MAX_ADDRESS_SIZE;
}

static void physical_page_set_virtual_address(physical_page_t* physical_page, uint64_t virtual_address) {
    physical_page->virtual_address = virtual_address;
}

size_t allocate_physical_address(size_t virtual_address) {
    if (g_available_pages == 0) {
        for (size_t i = 0; i < g_total_pages; i++) {
            physical_page_t* current_page = &g_phisycal_pages[i];
            if (!physical_page_is_available(current_page)) {
                unmap_page(current_page->virtual_address);
                physical_page_set_available(current_page);
                break;
            }
        }
    }

    for (size_t i = 0; i < g_total_pages; i++) {
        physical_page_t* current_page = &g_phisycal_pages[i];
        if (physical_page_is_available(current_page)) {
            physical_page_clear_available(current_page);
            physical_page_set_virtual_address(current_page, virtual_address);
            return current_page->address & 0xFFFFFFF00000;
        }
    }
}

void add_physical_pages(memory_chunk_t chunk) {
    size_t chunk_page_count = (chunk.end - chunk.start) / PAGE_SIZE;
    for (size_t i = 0; i < chunk_page_count; i++) {
        physical_page_t* current_page = &g_phisycal_pages[i];

        physical_page_set_address(current_page, i * PAGE_SIZE + chunk.start);
        physical_page_set_available(current_page);

        g_total_pages += 1;
        g_available_pages += 1;
    }
}

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

static page_table_entry_t* allocate_page_table() {
    return kalloc(sizeof(page_table_entry_t) * 512);
}

void unmap_page(size_t virtual_address) {
    printk(NONE, "Unapping: %x\n", virtual_address);
    size_t l4_offset = (virtual_address >> 39) & 0x1FF;
    size_t l3_offset = (virtual_address >> 30) & 0x1FF;
    size_t l2_offset = (virtual_address >> 21) & 0x1FF;

    page_table_entry_t* l4_address = &g_page_table_l4[l4_offset];
    page_table_entry_t* l3_address = &g_page_table_l3[l3_offset];
    page_table_entry_t* l2_address = &g_page_table_l2[l2_offset];

    page_table_entry_set_address(l3_address, 0);
    page_table_entry_set_address(l2_address, 0);

    page_table_entry_set_flags(l3_address, 0);
    page_table_entry_set_flags(l2_address, 0);

    page_table_entry_unmap(l3_address);
    page_table_entry_unmap(l2_address);

    g_available_pages += 1;

    flush_page();
}

void map_page(size_t physical_address, size_t virtual_address, size_t flags) {
    printk(NONE, "Mapping: %x to %x\n", virtual_address, physical_address);
    size_t l4_offset = (virtual_address >> 39) & 0x1FF;
    size_t l3_offset = (virtual_address >> 30) & 0x1FF;
    size_t l2_offset = (virtual_address >> 21) & 0x1FF;

    page_table_entry_t* l4_address = &g_page_table_l4[l4_offset];
    page_table_entry_t* l3_address = &g_page_table_l3[l3_offset];
    page_table_entry_t* l2_address = &g_page_table_l2[l2_offset];

    page_table_entry_set_address(l3_address, l2_address);
    page_table_entry_set_address(l2_address, physical_address);

    page_table_entry_set_flags(l3_address, 0x2);
    page_table_entry_set_flags(l2_address, flags);

    page_table_entry_map(l3_address);
    page_table_entry_map(l2_address);

    g_available_pages -= 1;

    flush_page();
}