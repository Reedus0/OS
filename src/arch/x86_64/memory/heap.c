#include "arch/x86_64/asm/heap.h"
#include "arch/x86_64/memory/memory.h"
#include "heap.h"

void heap_descriptor_set_address(heap_descriptor_t* heap_descriptor, uint64_t address) {
    heap_descriptor->address = address;
}

void heap_descriptor_set_size(heap_descriptor_t* heap_descriptor, uint64_t size) {
    heap_descriptor->size = size;
}

void heap_descriptor_set_available(heap_descriptor_t* heap_descriptor) {
    heap_descriptor->available |= ((size_t)1 << MAX_ADDRESS_SIZE);
}

void heap_descriptor_clear_available(heap_descriptor_t* heap_descriptor) {
    heap_descriptor->available &= heap_descriptor->available ^ (size_t)1 << MAX_ADDRESS_SIZE;
}

char heap_descriptor_is_available(heap_descriptor_t* heap_descriptor) {
    return heap_descriptor->available >> MAX_ADDRESS_SIZE;
}

void init_heap() {
    g_heap_descriptor_count = 0;

    heap_descriptor_t* base_descriptor = &g_heap_descriptors[g_heap_descriptor_count];
    heap_descriptor_set_address(base_descriptor, g_heap_base);
    heap_descriptor_set_size(base_descriptor, HEAP_SIZE);
    heap_descriptor_set_available(base_descriptor);

    g_heap_descriptor_count += 1;
} 

void insert_heap_descriptor(heap_descriptor_t new_descriptor) {
    for (size_t i = 0; i < g_heap_descriptor_count; i++) {
        heap_descriptor_t* current_descriptor = &g_heap_descriptors[i];
        if (current_descriptor->address > new_descriptor.address) {
            heap_descriptor_t tmp = *current_descriptor;
            g_heap_descriptors[i] = new_descriptor;
            g_heap_descriptors[i + 1] = tmp;
            g_heap_descriptor_count += 1;
            return;
        }
    }
    g_heap_descriptors[g_heap_descriptor_count] = new_descriptor;
    g_heap_descriptor_count += 1;
}

void* heap_alloc(size_t bytes) {
    bytes = bytes % 8 == 0 ? bytes : (((bytes / 8) + 1) * 8);

    for (size_t i = 0; i < g_heap_descriptor_count; i++) {
        heap_descriptor_t* current_descriptor = &g_heap_descriptors[i];
        printf("curr: 0x%x\n", *current_descriptor);
        if (!heap_descriptor_is_available(current_descriptor)) {
            continue;
        }
        if (current_descriptor->size >= bytes) {
            uint32_t old_size = current_descriptor->size;
            if (current_descriptor->size > bytes) {
                heap_descriptor_t new_descriptor;

                heap_descriptor_set_address(&new_descriptor, current_descriptor->address + bytes);
                heap_descriptor_set_size(&new_descriptor, old_size);
                heap_descriptor_set_available(&new_descriptor);

                printf("new 0x%x\n", new_descriptor);
                insert_heap_descriptor(new_descriptor);
            }
            heap_descriptor_set_size(current_descriptor, bytes);
            heap_descriptor_clear_available(current_descriptor);
            printf("count 0x%x\n", g_heap_descriptor_count);
            return current_descriptor->address;
        }
    }
    panic("Couldn't allocate memory!");
}

void heap_free(void* ptr) {
    if (ptr == NULL) {
        return;
    }
    for (size_t i = 0; i < g_heap_descriptor_count; i++) {
        heap_descriptor_t* current_descriptor = &g_heap_descriptors[i];
        if (current_descriptor->address == (uint64_t)ptr) {
            heap_descriptor_set_available(current_descriptor);
            return; 
        }
    }
    panic("Couldn't free memory!");
}