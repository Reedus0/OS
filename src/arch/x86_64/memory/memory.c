#include <stdio.h>
#include "memory.h"
#include "arch/x86_64/boot/multiboot2.h"

static size_t get_base_address(multiboot2_info_t* mbd) {
    multiboot2_tag_base_address_t* base_address_tag = get_tag(mbd, TAG_BASE_ADDRESS);

    return base_address_tag->base_address;
}

static memory_chunk_t get_memory_chunk(multiboot2_memory_map_t* map, size_t base_address) {
    memory_chunk_t result;

    result.start = map->address;
    result.end = map->length + result.start;

    if (result.start == base_address) {
        result.start += KERNEL_OFFSET;
    }
    
    return result;
}

void discover_memory(multiboot2_info_t* mbd) {
    size_t base_address = get_base_address(mbd);

    multiboot2_tag_mmap_t* mmap_tag = get_tag(mbd, TAG_MMAP);
    size_t map_count = (mmap_tag->size - sizeof(multiboot2_tag_mmap_t)) / sizeof(multiboot2_memory_map_t);

    for (size_t i = 0; i < map_count; i++) {
        multiboot2_memory_map_t* map = &mmap_tag->entries + i;

        if(map->type == MEMORY_AVAILABLE && map->address != 0) {
            memory_chunk_t chunk = get_memory_chunk(map, base_address);

            g_memory_list[g_memory_chunk_count] = chunk;

            size_t map_size = chunk.end - chunk.start;

            g_available_memory += map_size;
            g_memory_chunk_count += 1;
        }
    }

    printf("Available memory: 0x%x\n", g_available_memory);
}