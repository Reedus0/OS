#include "memory.h"
#include "paging.h"
#include "asm/paging.h"
#include "memory/heap.h"

static void physical_page_set_address(physical_page_t* physical_page, uint64_t address) {
    physical_page->address = address;
}

static void physical_page_set_available(physical_page_t* physical_page) {
    physical_page->available = 1;
}

static void physical_page_clear_available(physical_page_t* physical_page) {
    physical_page->available = 0;
}

static char physical_page_is_available(physical_page_t* physical_page) {
    return physical_page->available;
}

size_t find_physical_page() {
    if (g_available_pages == 0) {
        // TODO: SWAP
    }

    for (size_t i = 0; i < g_total_pages; i++) {
        physical_page_t* current_page = &g_physical_pages[i];
        if (physical_page_is_available(current_page)) {
            physical_page_clear_available(current_page);
            return current_page->address;
        }
    }
}

void allocate_physical_page(size_t physical_address) {
    for (size_t i = 0; i < g_total_pages; i++) {
        physical_page_t* current_page = &g_physical_pages[i];
        if (current_page->address == physical_address && physical_page_is_available(current_page)) {
            physical_page_clear_available(current_page);

            g_available_pages -= 1;
            return;
        }
    }
    panic("Couldn't allocate physical page!");
}

void free_physical_page(size_t physical_address) {
    for (size_t i = 0; i < g_total_pages; i++) {
        physical_page_t* current_page = &g_physical_pages[i];
        if (current_page->address == physical_address) {
            physical_page_set_available(current_page);

            g_available_pages += 1;
            return;
        }
    }
    panic("Couldn't free physical page!");
}

void add_physical_pages(memory_chunk_t chunk) {
    size_t chunk_page_count = (chunk.end - chunk.start) / PAGE_SIZE;
    for (size_t i = 0; i < chunk_page_count; i++) {
        physical_page_t* current_page = &g_physical_pages[g_total_pages];

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
    page_table_entry->address ^= (page_table_entry->address & 0xFFFFFFFFFFFFF000);
    page_table_entry->address |= (uint64_t)address & 0xFFFFFFFFFFFFF000;
}

static void page_table_entry_map(page_table_entry_t* page_table_entry) {
    page_table_entry->present |= 1;
}

static void page_table_entry_unmap(page_table_entry_t* page_table_entry) {
    page_table_entry->present ^= page_table_entry->present & 1;
}

static page_table_descriptor_t* allocate_page_table(byte level, uint32_t id) {
    page_table_descriptor_t* descriptor = kalloc(sizeof(page_table_descriptor_t));

    descriptor->level = level;
    descriptor->id = id;
    descriptor->table = kalloc_aligned(sizeof(page_table_entry_t) * 512, 4096);

    g_page_table_descriptors[g_page_table_descriptors_size] = descriptor;
    g_page_table_descriptors_size += 1;

    return descriptor;
}

static byte free_page_table(page_table_descriptor_t* descriptor) {
    for (size_t i = 0; i < 512; i++) {
        if (descriptor->table[i].present & 1) return 1;
    }

    for (size_t i = 0; i < g_page_table_descriptors_size; i++) {
        if (g_page_table_descriptors[i] == descriptor) {
            for (size_t j = i; j < g_page_table_descriptors_size - 1; j++) {
                g_page_table_descriptors[j] = g_page_table_descriptors[j + 1];
            }
            g_page_table_descriptors_size--;
            break;
        }
    }

    kfree(descriptor->table);
    kfree(descriptor);

    return 0;
}

static page_table_descriptor_t* get_table_descriptor(byte level, uint32_t id) {
    for (size_t i = 0; i < g_page_table_descriptors_size; i++) {
        page_table_descriptor_t* current_descriptor = g_page_table_descriptors[i];
        if (current_descriptor->level == level && current_descriptor->id == id) {
            return current_descriptor;
        }
    }
    return 0;
}

static void map_kernel() {
    for (size_t i = 0; i < 120; i++) {
        map_page(0x1000000 + 0x200000 * i, 0xFFFF800001000000 + 0x200000 * i, 0x82);
    }
}

static void map_bottom() {
    for (size_t i = 0; i < 2048; i++) {
        map_page(0x200000 * i, 0x200000 * i, 0x82);
    }
}

static void map_heap() {
    for (size_t i = 0; i < 1792; i++) {
        map_page(0x10000000 + 0x200000 * i, 0xFFFF800010000000 + 0x200000 * i, 0x82);
    }
}

void init_paging() {
    map_kernel();
    map_bottom();
    map_heap();

    page_table_descriptor_t* l4_descriptor = get_table_descriptor(4, 0);
    set_page_directory(get_physical_address(l4_descriptor->table));
}

size_t get_physical_address(size_t virtual_address) {
    size_t l4_offset = (virtual_address >> 39) & 0x1FF;
    size_t l3_offset = (virtual_address >> 30) & 0x1FF;
    size_t l2_offset = (virtual_address >> 21) & 0x1FF;

    page_table_entry_t* l4_address = get_page_directory();
    page_table_entry_t l4_entry = l4_address[l4_offset];

    page_table_entry_t* l3_address = (page_table_entry_t*)(l4_entry.address & 0xFFFFFFFFFFFFF000);
    page_table_entry_t l3_entry = l3_address[l3_offset];

    page_table_entry_t* l2_address = (page_table_entry_t*)(l3_entry.address & 0xFFFFFFFFFFFFF000);
    page_table_entry_t l2_entry = l2_address[l2_offset];

    return (l2_entry.address & 0xFFFFFFFFFFF00000) | (virtual_address & 0xFFFFF);
}

void map_page(size_t physical_address, size_t virtual_address, size_t flags) {
    size_t l4_offset = (virtual_address >> 39) & 0x1FF;
    size_t l3_offset = (virtual_address >> 30) & 0x1FF;
    size_t l2_offset = (virtual_address >> 21) & 0x1FF;

    page_table_descriptor_t* l4_descriptor = get_table_descriptor(4, 0);
    page_table_descriptor_t* l3_descriptor = get_table_descriptor(3, ((l4_offset << 9)));
    page_table_descriptor_t* l2_descriptor = get_table_descriptor(2, ((l3_offset << 18) | (l4_offset << 9)));

    if (!l4_descriptor) l4_descriptor = allocate_page_table(4, 0);
    if (!l3_descriptor) l3_descriptor = allocate_page_table(3, ((l4_offset << 9)));
    if (!l2_descriptor) l2_descriptor = allocate_page_table(2, ((l3_offset << 18) | (l4_offset << 9)));

    page_table_entry_t* l4_address = get_physical_address(&l4_descriptor->table[l4_offset]);
    page_table_entry_t* l3_address = get_physical_address(&l3_descriptor->table[l3_offset]);
    page_table_entry_t* l2_address = get_physical_address(&l2_descriptor->table[l2_offset]);

    page_table_entry_set_address(l4_address, l3_address);
    page_table_entry_set_address(l3_address, l2_address);
    page_table_entry_set_address(l2_address, physical_address);

    page_table_entry_set_flags(l4_address, 0x2);
    page_table_entry_set_flags(l3_address, 0x2);
    page_table_entry_set_flags(l2_address, flags);

    page_table_entry_map(l4_address);
    page_table_entry_map(l3_address);
    page_table_entry_map(l2_address);

    flush_page();
}

void unmap_page(size_t virtual_address) {
    size_t l4_offset = (virtual_address >> 39) & 0x1FF;
    size_t l3_offset = (virtual_address >> 30) & 0x1FF;
    size_t l2_offset = (virtual_address >> 21) & 0x1FF;

    page_table_descriptor_t* l4_descriptor = get_table_descriptor(4, 0);
    page_table_descriptor_t* l3_descriptor = get_table_descriptor(3, ((l4_offset << 9)));
    page_table_descriptor_t* l2_descriptor = get_table_descriptor(2, ((l3_offset << 18) | (l4_offset << 9)));

    page_table_entry_t* l2_address = get_physical_address(&l2_descriptor->table[l2_offset]);
    page_table_entry_unmap(l2_address);

    if (!free_page_table(l2_descriptor)) {
        page_table_entry_t* l3_address = get_physical_address(&l3_descriptor->table[l3_offset]);
        page_table_entry_unmap(l3_address);

        if (!free_page_table(l3_descriptor)) {
            page_table_entry_t* l4_address = get_physical_address(&l4_descriptor->table[l4_offset]);
            page_table_entry_unmap(l4_address);

            free_page_table(l4_descriptor);
        }
    }

    flush_page();
}

static print_page_table(page_table_entry_t* table, uint8_t level) {
    if (!table) return;
    if (level < 2) return;

    for (size_t i = 0; i < 512; i++) {
        page_table_entry_t* current_entry = table + i;
        if (current_entry->address && current_entry->present & 1) {
            for (size_t i = 0; i < 4 - level; i++) printk(NONE, "  ");
            printk(NONE, "- %d: %x\n", i, current_entry->address & 0xFFFFFFFFF000);
            print_page_table(current_entry->address & 0xFFFFFFFFF000, level - 1);
        }
    }
}

void print_pages(page_table_entry_t* table) {
    printk(NONE, "Page table:\n");
    print_page_table(table, 4);
}