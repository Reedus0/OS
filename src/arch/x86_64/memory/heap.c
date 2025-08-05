#include "kernel/printk.h"
#include "memory/memory.h"
#include "memory/paging.h"
#include "heap.h"

static void heap_descriptor_set_address(heap_descriptor_t* heap_descriptor, uint64_t address) {
    heap_descriptor->address = address;
}

static void heap_descriptor_set_size(heap_descriptor_t* heap_descriptor, uint64_t size) {
    heap_descriptor->size = size;
}

static void heap_descriptor_set_available(heap_descriptor_t* heap_descriptor) {
    heap_descriptor->available |= ((size_t)1 << MAX_ADDRESS_SIZE);
}

static void heap_descriptor_clear_available(heap_descriptor_t* heap_descriptor) {
    heap_descriptor->available &= heap_descriptor->available ^ (size_t)1 << MAX_ADDRESS_SIZE;
}

static char heap_descriptor_is_available(heap_descriptor_t* heap_descriptor) {
    return heap_descriptor->available >> MAX_ADDRESS_SIZE;
}

void init_heap() {
    g_heap_descriptor_count = 0;

    heap_descriptor_t* base_descriptor = &g_heap_descriptors[g_heap_descriptor_count];
    heap_descriptor_set_address(base_descriptor, g_heap_base);
    heap_descriptor_set_size(base_descriptor, PAGE_SIZE * g_available_pages);
    heap_descriptor_set_available(base_descriptor);

    g_heap_descriptor_count += 1;
}

void print_heap() {
    printk(NONE, "Heap:\n");
    for (size_t i = 0; i < g_heap_descriptor_count; i++) {
        heap_descriptor_t* current_descriptor = &g_heap_descriptors[i];
        printk(NONE, "desc: %x size: %x\n", current_descriptor->address, current_descriptor->size);
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

static void insert_heap_descriptor(heap_descriptor_t new_descriptor) {
    for (size_t i = 0; i < g_heap_descriptor_count; i++) {
        heap_descriptor_t* current_descriptor = &g_heap_descriptors[i];
        if ((current_descriptor->address & MAX_ADDRESS_MASK) > (new_descriptor.address & MAX_ADDRESS_MASK)) {
            shift_heap_descriptors_right(i);
            g_heap_descriptors[i] = new_descriptor;
            g_heap_descriptor_count += 1;
            return;
        }
    }
    g_heap_descriptors[g_heap_descriptor_count] = new_descriptor;
    g_heap_descriptor_count += 1;
}

void* heap_alloc(size_t bytes) {
    if (bytes == 0) {
        panic("Tying to allocate 0 bytes!");
    }

    bytes = bytes % 8 == 0 ? bytes : (((bytes / 8) + 1) * 8);

    for (size_t i = 0; i < g_heap_descriptor_count; i++) {
        heap_descriptor_t* current_descriptor = &g_heap_descriptors[i];

        if (!heap_descriptor_is_available(current_descriptor)) {
            continue;
        }

        if (current_descriptor->size >= bytes) {
            uint32_t old_size = current_descriptor->size;
            if (current_descriptor->size > bytes) {
                heap_descriptor_t new_descriptor;

                heap_descriptor_set_address(&new_descriptor, current_descriptor->address + bytes);
                heap_descriptor_set_size(&new_descriptor, old_size - bytes);
                heap_descriptor_set_available(&new_descriptor);

                insert_heap_descriptor(new_descriptor);
            }
            heap_descriptor_set_size(current_descriptor, bytes);
            heap_descriptor_clear_available(current_descriptor);
            return current_descriptor->address;
        }
    }
    print_heap();
    panic("Couldn't allocate memory!");
}

void* heap_alloc_aligned(size_t bytes, size_t alignment) {
    if (bytes == 0 || alignment == 0 || (alignment & (alignment - 1)) != 0) {
        panic("Invalid allocation size or alignment!");
    }

    bytes = bytes % 8 == 0 ? bytes : (((bytes / 8) + 1) * 8);

    for (size_t i = 0; i < g_heap_descriptor_count; i++) {
        heap_descriptor_t* current_descriptor = &g_heap_descriptors[i];

        if (!heap_descriptor_is_available(current_descriptor)) {
            continue;
        }

        uint64_t real_address = current_descriptor->address & MAX_ADDRESS_MASK;
        uint64_t aligned_address = (real_address + alignment - 1) & ~(alignment - 1);
        uint64_t padding = aligned_address - real_address;

        if (current_descriptor->size < bytes + padding) {
            continue;
        }

        if (padding > 0) {
            heap_descriptor_t padding_descriptor;

            heap_descriptor_set_address(&padding_descriptor, real_address);
            heap_descriptor_set_size(&padding_descriptor, padding);
            heap_descriptor_set_available(&padding_descriptor);

            insert_heap_descriptor(padding_descriptor);
        }

        uint64_t remaining = current_descriptor->size - (padding + bytes);
        if (remaining > 0) {
            heap_descriptor_t tail_descriptor;

            heap_descriptor_set_address(&tail_descriptor, aligned_address + bytes);
            heap_descriptor_set_size(&tail_descriptor, remaining);
            heap_descriptor_set_available(&tail_descriptor);

            insert_heap_descriptor(tail_descriptor);
        }

        heap_descriptor_set_address(current_descriptor, aligned_address);
        heap_descriptor_set_size(current_descriptor, bytes);
        heap_descriptor_clear_available(current_descriptor);
        print_heap();

        return current_descriptor->address;
    }

    print_heap();
    panic("Couldn't allocate aligned memory!");
}


static void heap_descriptor_clear_memory(heap_descriptor_t* available_descriptor) {
    memset(available_descriptor->address & MAX_ADDRESS_MASK, 0, available_descriptor->size);
}

static void merge_descriptors(heap_descriptor_t* available_descriptor) {
    for (size_t i = 0; i < g_heap_descriptor_count; i++) {
        heap_descriptor_t* current_descriptor = &g_heap_descriptors[i];
        if (!heap_descriptor_is_available(current_descriptor)) {
            continue;
        }

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
            heap_descriptor_set_available(current_descriptor);
            heap_descriptor_clear_memory(current_descriptor);
            merge_descriptors(current_descriptor);
            return;
        }
    }
    print_heap();
    panic("Couldn't free memory!");
}