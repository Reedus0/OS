#include <stdio.h>
#include "memory.h"

void discover_memory(multiboot2_info_t* mbd) {
    size_t i = 0;
    size_t base_address = 0;
    while(i < mbd->size) {
        multiboot2_tag_t* current_tag = &mbd->tags + i; 
        if(current_tag->type == TAG_BASE_ADDRESS) {
            multiboot2_tag_base_address_t* base_address_tag = &mbd->tags + i;
            base_address = base_address_tag->base_address;
        }
        if(current_tag->type == TAG_MMAP) {
            break;
        }
        i = i + current_tag->size;
    }
    printf("Base address: 0x%x\n", base_address);
    multiboot2_tag_mmap_t* mmap_tag = &mbd->tags + i;
    for (size_t i = 0; i < (mmap_tag->size - 16) / sizeof(multiboot2_memory_map_t); i++) {
        multiboot2_memory_map_t* map = &mmap_tag->entries + i;
        printf("Addr: 0x%x, Type: 0x%x, Length: 0x%x, Zero: 0x%x\n", map->address, map->type, map->length, map->zero);
    }
    while(1);
}