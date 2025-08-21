#include "kernel/io.h"
#include "kernel/panic.h"
#include "memory/memory.h"
#include "memory/paging.h"
#include "heap.h"

static void heap_descriptor_set_address(heap_descriptor_t* heap_descriptor, uint64_t address) {
    heap_descriptor->address = address;
}

static void heap_descriptor_set_page_address(heap_descriptor_t* heap_descriptor, uint64_t page_address) {
    heap_descriptor->page_address = page_address;
}


static void heap_descriptor_set_size(heap_descriptor_t* heap_descriptor, uint64_t size) {
    heap_descriptor->size = size;
}

static void heap_descriptor_set_available(heap_descriptor_t* heap_descriptor) {
    heap_descriptor->available = 1;
}

static void heap_descriptor_clear_available(heap_descriptor_t* heap_descriptor) {
    heap_descriptor->available = 0;
}

static uint8_t heap_descriptor_is_available(heap_descriptor_t* heap_descriptor) {
    return heap_descriptor->available;
}

static size_t heap_descriptor_calculate_canary(heap_descriptor_t* heap_descriptor) {
    uint64_t canary = heap_descriptor->address;
    canary ^= ((uint64_t)heap_descriptor->size << 32) | heap_descriptor->size;
    canary ^= canary >> 33;
    canary *= 0xFF51AFD7ED558CCD;
    canary ^= canary >> 33;
    canary *= 0xC4CEB9FE1A85EC53;
    canary ^= canary >> 33;
    return canary;
}

static void heap_descriptor_set_canary(heap_descriptor_t* heap_descriptor) {
    size_t canary = heap_descriptor_calculate_canary(heap_descriptor);
    *(size_t*)(heap_descriptor->address + heap_descriptor->size - HEAP_CANARY_SIZE) = canary;
}

static uint8_t heap_descriptor_check_canary(heap_descriptor_t* heap_descriptor) {
    size_t canary = heap_descriptor_calculate_canary(heap_descriptor);
    size_t real_canary = *(size_t*)(heap_descriptor->address + heap_descriptor->size - HEAP_CANARY_SIZE);

    return canary == real_canary;
}

void print_heap() {
    printk(NONE, "Heap:\n");
    for (size_t i = 0; i < g_heap_descriptor_count; i++) {
        heap_descriptor_t* current_descriptor = &g_heap_descriptors[i];
        printk(NONE, "desc: 0x%16x size: 0x%8x page: 0x%16x available: %x\n", current_descriptor->address, current_descriptor->size, current_descriptor->page_address, current_descriptor->available);
    }
}

static void shift_heap_descriptors_right(size_t index) {
    for (size_t i = g_heap_descriptor_count - 1; i > index - 1; i--) {
        heap_descriptor_t current_descriptor = g_heap_descriptors[i];
        g_heap_descriptors[i + 1] = current_descriptor;
    }
}

static void shift_heap_descriptors_left(size_t index) {
    for (size_t i = index + 1; i < g_heap_descriptor_count - 1; i++) {
        heap_descriptor_t current_descriptor = g_heap_descriptors[i + 1];
        g_heap_descriptors[i] = current_descriptor;
    }
}

static void insert_heap_descriptor(heap_descriptor_t* new_descriptor) {
    for (size_t i = 0; i < g_heap_descriptor_count; i++) {
        heap_descriptor_t* current_descriptor = &g_heap_descriptors[i];
        if ((current_descriptor->address) > (new_descriptor->address)) {
            shift_heap_descriptors_right(i);
            g_heap_descriptors[i] = *new_descriptor;
            g_heap_descriptor_count += 1;
            return;
        }
    }
    g_heap_descriptors[g_heap_descriptor_count] = *new_descriptor;
    g_heap_descriptor_count += 1;
}

static void remove_heap_descriptor(heap_descriptor_t* heap_descriptor) {
    for (size_t i = 0; i < g_heap_descriptor_count; i++) {
        heap_descriptor_t* current_descriptor = &g_heap_descriptors[i];
        if (current_descriptor->address == heap_descriptor->address) {
            shift_heap_descriptors_left(i - 1);
            g_heap_descriptor_count -= 1;
            return;
        }
    }
    panic("Couldn't remove heap descriptor!");
}

heap_descriptor_t allocate_heap_descriptor() {
    size_t physical_page = allocate_physical_page();

    heap_descriptor_t new_descriptor;

    if (g_heap_descriptor_count == 0) {
        heap_descriptor_set_address(&new_descriptor, HEAP_PHYSICAL_ADDRESS_OFFSET + 0xFFFF800000000000);
    }
    else {
        heap_descriptor_t* last_descriptor = &g_heap_descriptors[g_heap_descriptor_count - 1];
        size_t new_address = last_descriptor->address + last_descriptor->size;

        heap_descriptor_set_address(&new_descriptor, new_address);
    }
    heap_descriptor_set_size(&new_descriptor, PAGE_SIZE);
    heap_descriptor_set_available(&new_descriptor);
    heap_descriptor_set_page_address(&new_descriptor, physical_page);

    return new_descriptor;
}

void free_heap_descriptor(heap_descriptor_t* heap_descriptor) {
    free_physical_page(heap_descriptor->page_address);

    remove_heap_descriptor(heap_descriptor);
}

void* heap_alloc(size_t bytes) {
    if (bytes == 0) {
        panic("Tying to allocate 0 bytes!");
    }

    bytes = align(bytes, 8);
    bytes = bytes + HEAP_CANARY_SIZE;

    for (size_t i = 0; i < g_heap_descriptor_count; i++) {
        heap_descriptor_t* current_descriptor = &g_heap_descriptors[i];

        if (!heap_descriptor_is_available(current_descriptor)) {
            continue;
        }

        if (current_descriptor->size < bytes) {
            continue;
        }

        if (current_descriptor->size == bytes) {
            heap_descriptor_set_size(current_descriptor, bytes);
            heap_descriptor_clear_available(current_descriptor);
            heap_descriptor_set_canary(current_descriptor);

            return current_descriptor->address;
        }

        uint64_t old_size = current_descriptor->size;
        heap_descriptor_t new_descriptor;

        heap_descriptor_set_address(&new_descriptor, current_descriptor->address + bytes);
        heap_descriptor_set_size(&new_descriptor, old_size - bytes);
        heap_descriptor_set_available(&new_descriptor);
        heap_descriptor_set_page_address(&new_descriptor, current_descriptor->page_address);

        insert_heap_descriptor(&new_descriptor);

        heap_descriptor_set_size(current_descriptor, bytes);
        heap_descriptor_clear_available(current_descriptor);
        heap_descriptor_set_canary(current_descriptor);

        return current_descriptor->address;
    }

    heap_descriptor_t new_descriptor = allocate_heap_descriptor();
    insert_heap_descriptor(&new_descriptor);

    return heap_alloc(bytes);
}

void* heap_alloc_aligned(size_t bytes, size_t alignment) {
    if (bytes == 0 || alignment == 0 || (alignment & (alignment - 1)) != 0) {
        panic("Invalid allocation size or alignment!");
    }

    bytes = (bytes + 7) & ~7;
    bytes = bytes + HEAP_CANARY_SIZE;

    for (size_t i = 0; i < g_heap_descriptor_count; i++) {
        heap_descriptor_t* current_descriptor = &g_heap_descriptors[i];

        if (!heap_descriptor_is_available(current_descriptor)) {
            continue;
        }

        uint64_t real_address = current_descriptor->address;
        uint64_t aligned_address = (real_address + alignment - 1) & ~(alignment - 1);
        uint64_t padding = aligned_address - real_address;

        if (current_descriptor->size < bytes + padding) {
            continue;
        }

        uint64_t remaining = current_descriptor->size - (padding + bytes);

        if (padding == 0) {
            if (remaining == 0) {
                heap_descriptor_set_size(current_descriptor, bytes);
                heap_descriptor_clear_available(current_descriptor);
                heap_descriptor_set_canary(current_descriptor);

                return real_address;
            }
            heap_descriptor_t new_descriptor;

            heap_descriptor_set_address(&new_descriptor, real_address + bytes);
            heap_descriptor_set_size(&new_descriptor, remaining);
            heap_descriptor_set_available(&new_descriptor);
            heap_descriptor_set_page_address(&new_descriptor, current_descriptor->page_address);

            insert_heap_descriptor(&new_descriptor);

            heap_descriptor_set_size(current_descriptor, bytes);
            heap_descriptor_clear_available(current_descriptor);
            heap_descriptor_set_canary(current_descriptor);

            return real_address;
        }

        if (remaining) {
            heap_descriptor_t new_descriptor;

            heap_descriptor_set_address(&new_descriptor, aligned_address + bytes);
            heap_descriptor_set_size(&new_descriptor, remaining);
            heap_descriptor_set_available(&new_descriptor);
            heap_descriptor_set_page_address(&new_descriptor, current_descriptor->page_address);

            insert_heap_descriptor(&new_descriptor);
        }

        heap_descriptor_set_size(current_descriptor, padding);

        heap_descriptor_t new_descriptor;
        heap_descriptor_set_address(&new_descriptor, aligned_address);
        heap_descriptor_set_size(&new_descriptor, bytes);
        heap_descriptor_clear_available(&new_descriptor);
        heap_descriptor_set_canary(&new_descriptor);
        heap_descriptor_set_page_address(&new_descriptor, current_descriptor->page_address);

        insert_heap_descriptor(&new_descriptor);

        return new_descriptor.address;
    }

    heap_descriptor_t new_descriptor = allocate_heap_descriptor();
    insert_heap_descriptor(&new_descriptor);

    return heap_alloc_aligned(bytes, alignment);
}


static void heap_descriptor_clear_memory(heap_descriptor_t* available_descriptor) {
    for (size_t i = 0; i < available_descriptor->size; i++) {
        *(volatile char*)(available_descriptor->address + i) = 0;
    }
}

static void merge_descriptors(heap_descriptor_t* available_descriptor) {
    for (size_t i = 0; i < g_heap_descriptor_count; i++) {
        heap_descriptor_t* current_descriptor = &g_heap_descriptors[i];
        if (!heap_descriptor_is_available(current_descriptor)) {
            continue;
        }

        if (current_descriptor->page_address != available_descriptor->page_address) continue;

        if (current_descriptor->address + current_descriptor->size == available_descriptor->address) {
            current_descriptor->size += available_descriptor->size;
            available_descriptor = current_descriptor;
            shift_heap_descriptors_left(i);
            g_heap_descriptor_count -= 1;
            continue;
        }

        if (available_descriptor->address + available_descriptor->size == current_descriptor->address) {
            available_descriptor->size += current_descriptor->size;
            shift_heap_descriptors_left(i - 1);
            g_heap_descriptor_count -= 1;
            continue;
        }
    }
}

void heap_free(void* ptr) {
    if (ptr == NULL) {
        return;
    }
    for (size_t i = 0; i < g_heap_descriptor_count; i++) {
        heap_descriptor_t* current_descriptor = &g_heap_descriptors[i];
        if (current_descriptor->address == (uint64_t)ptr) {
            if (!heap_descriptor_check_canary(current_descriptor)) panic("Heap smashing detected!");

            heap_descriptor_t* prev_descriptor = &g_heap_descriptors[i - 1];
            heap_descriptor_t* next_descriptor = &g_heap_descriptors[i + 1];

            if (prev_descriptor->address != current_descriptor->address && next_descriptor->address != current_descriptor->address) {
                merge_descriptors(current_descriptor);
                free_heap_descriptor(current_descriptor);
            }
            else {
                heap_descriptor_set_available(current_descriptor);
                heap_descriptor_clear_memory(current_descriptor);
                merge_descriptors(current_descriptor);
            }

            return;
        }
    }
    print_heap();
    panic("Couldn't free memory!");
}