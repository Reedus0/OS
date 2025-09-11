#include "paging.h"
#include "discover.h"
#include "boot/multiboot2.h"

static size_t validate_chunk(multiboot2_memory_map_t* map) {
    uint64_t start = map->address;
    uint64_t end = map->length + start;

    size_t memory_is_available = map->type == MEMORY_AVAILABLE;
    size_t not_zero_address = map->address != 0;
    size_t big_enough = (end - start) >= PAGE_SIZE;

    return memory_is_available && not_zero_address && big_enough;
}

static memory_chunk_t get_memory_chunk(multiboot2_memory_map_t* map) {
    memory_chunk_t result;

    result.start = align(map->address, PAGE_SIZE);
    result.end = align(map->length + result.start, PAGE_SIZE);

    return result;
}

static void process_map(multiboot2_memory_map_t* map) {
    if (validate_chunk(map)) {
        memory_chunk_t chunk = get_memory_chunk(map);

        if (chunk.start == 0) {
            return;
        }

        add_physical_pages(chunk);
    }
}

void discover_memory(multiboot2_info_t* mbd) {
    multiboot2_tag_mmap_t* mmap_tag = get_tag(mbd, TAG_MMAP);
    size_t map_count = (mmap_tag->size - sizeof(multiboot2_tag_mmap_t)) / sizeof(multiboot2_memory_map_t);

    for (size_t i = 0; i < map_count; i++) {
        multiboot2_memory_map_t* map = &mmap_tag->entries + i;

        process_map(map);
    }
}