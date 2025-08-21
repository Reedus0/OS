#include "kernel/io.h"
#include "kernel/panic.h"
#include "memory/memory.h"
#include "memory/paging.h"
#include "heap.h"

static void heap_page_set_address(heap_page_t* heap_page, uint64_t address) {
    heap_page->address = address;
}

static void heap_page_set_page_address(heap_page_t* heap_page, uint64_t page_address) {
    heap_page->page_address = page_address;
}

static void heap_page_set_available(heap_page_t* heap_page) {
    heap_page->available = 1;
}

static void heap_page_clear_available(heap_page_t* heap_page) {
    heap_page->available = 0;
}

static void heap_descriptor_set_page(heap_descriptor_t* heap_descriptor, heap_page_t* page) {
    heap_descriptor->page = page;
}

static void heap_descriptor_set_address(heap_descriptor_t* heap_descriptor, uint64_t address) {
    heap_descriptor->address = address;
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
        printk(NONE, "desc: 0x%16x size: 0x%8x page: 0x%16x available: %x\n", current_descriptor->address, current_descriptor->size, current_descriptor->page->page_address, current_descriptor->available);
    }
    printk(NONE, "Descriptors: %x Pages: %x\n", g_heap_descriptor_count, g_heap_pages_count);
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

heap_page_t* allocate_heap_page(size_t count) {
    size_t base_address = HEAP_OFFSET + 0xFFFF800000000000;

    for (size_t i = 0; i < g_heap_pages_count; i++) {
        heap_page_t* current_page = &g_heap_pages[i];

        if (current_page->available) {

            size_t available_count = 1;
            for (size_t j = 0; j < count - 1; j++) {
                heap_page_t* next_page = &g_heap_pages[i + j];
                if (next_page->available) available_count += 1;
            }

            if (available_count == count) {
                size_t physical_page = allocate_physical_page();

                heap_page_set_address(current_page, base_address + i * PAGE_SIZE);
                heap_page_set_page_address(current_page, physical_page);
                heap_page_clear_available(current_page);

                for (size_t j = 0; j < count; j++) {
                    size_t physical_page = allocate_physical_page();

                    heap_page_t* next_page = &g_heap_pages[i + j];
                    heap_page_set_address(next_page, base_address + (i + j) * PAGE_SIZE);
                    heap_page_set_page_address(next_page, physical_page);
                    heap_page_clear_available(next_page);

                    list_insert_after(&current_page->list, &next_page->list);
                }

                return current_page;
            }
        }
    }

    heap_page_t* current_page = &g_heap_pages[g_heap_pages_count];
    size_t physical_page = allocate_physical_page();

    heap_page_set_address(current_page, base_address + g_heap_pages_count * PAGE_SIZE);
    heap_page_set_page_address(current_page, physical_page);
    heap_page_clear_available(current_page);

    g_heap_pages_count += 1;

    for (size_t j = 0; j < count - 1; j++) {
        size_t physical_page = allocate_physical_page();

        heap_page_t* next_page = &g_heap_pages[g_heap_pages_count + j];
        heap_page_set_address(next_page, base_address + (g_heap_pages_count + j) * PAGE_SIZE);
        heap_page_set_page_address(next_page, physical_page);
        heap_page_clear_available(next_page);

        list_insert_after(&current_page->list, &next_page->list);
        g_heap_pages_count += 1;
    }

    return current_page;
}

void free_heap_page(heap_page_t* heap_page) {
    while (1) {
        list_t* next = heap_page->list.next;

        free_physical_page(heap_page->page_address);
        heap_page_set_address(heap_page, 0);
        heap_page_set_page_address(heap_page, 0);
        heap_page_set_available(heap_page);

        if (next == NULL) {
            break;
        }
        heap_page = container_of(next, heap_page_t, list);
    }
}

void* heap_alloc(size_t bytes) {
    if (bytes == 0) {
        panic("Tying to allocate 0 bytes!");
    }

    bytes = align(bytes, 8);
    bytes = bytes + HEAP_CANARY_SIZE;

    if (bytes > PAGE_SIZE) {
        size_t pages_count = (bytes / PAGE_SIZE) + 1;
        heap_page_t* new_page = allocate_heap_page(pages_count);
        heap_descriptor_t new_descriptor;

        heap_descriptor_set_address(&new_descriptor, new_page->address);
        heap_descriptor_set_size(&new_descriptor, PAGE_SIZE * pages_count);
        heap_descriptor_set_available(&new_descriptor);
        heap_descriptor_set_page(&new_descriptor, new_page);

        insert_heap_descriptor(&new_descriptor);
    }

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
        heap_descriptor_set_page(&new_descriptor, current_descriptor->page);

        insert_heap_descriptor(&new_descriptor);

        heap_descriptor_set_size(current_descriptor, bytes);
        heap_descriptor_clear_available(current_descriptor);
        heap_descriptor_set_canary(current_descriptor);

        return current_descriptor->address;
    }

    heap_page_t* new_page = allocate_heap_page(1);
    heap_descriptor_t new_descriptor;

    heap_descriptor_set_address(&new_descriptor, new_page->address);
    heap_descriptor_set_size(&new_descriptor, PAGE_SIZE);
    heap_descriptor_set_available(&new_descriptor);
    heap_descriptor_set_page(&new_descriptor, new_page);

    insert_heap_descriptor(&new_descriptor);

    return heap_alloc(bytes);
}

void* heap_alloc_aligned(size_t bytes, size_t alignment) {
    if (bytes == 0 || alignment == 0 || (alignment & (alignment - 1)) != 0) {
        panic("Invalid allocation size or alignment!");
    }

    bytes = (bytes + 7) & ~7;
    bytes = bytes + HEAP_CANARY_SIZE;

    if (bytes > PAGE_SIZE) {
        size_t pages_count = (bytes / PAGE_SIZE) + 1;
        heap_page_t* new_page = allocate_heap_page(pages_count);
        heap_descriptor_t new_descriptor;

        heap_descriptor_set_address(&new_descriptor, new_page->address);
        heap_descriptor_set_size(&new_descriptor, PAGE_SIZE * pages_count);
        heap_descriptor_set_available(&new_descriptor);
        heap_descriptor_set_page(&new_descriptor, new_page);

        insert_heap_descriptor(&new_descriptor);
    }

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
            heap_descriptor_set_page(&new_descriptor, current_descriptor->page);

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
            heap_descriptor_set_page(&new_descriptor, current_descriptor->page);

            insert_heap_descriptor(&new_descriptor);
        }

        heap_descriptor_set_size(current_descriptor, padding);

        heap_descriptor_t new_descriptor;
        heap_descriptor_set_address(&new_descriptor, aligned_address);
        heap_descriptor_set_size(&new_descriptor, bytes);
        heap_descriptor_clear_available(&new_descriptor);
        heap_descriptor_set_canary(&new_descriptor);
        heap_descriptor_set_page(&new_descriptor, current_descriptor->page);

        insert_heap_descriptor(&new_descriptor);

        return new_descriptor.address;
    }

    heap_page_t* new_page = allocate_heap_page(1);
    heap_descriptor_t new_descriptor;

    heap_descriptor_set_address(&new_descriptor, new_page->address);
    heap_descriptor_set_size(&new_descriptor, PAGE_SIZE);
    heap_descriptor_set_available(&new_descriptor);
    heap_descriptor_set_page(&new_descriptor, new_page);

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

        if (current_descriptor->page != available_descriptor->page) continue;

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

            heap_descriptor_set_available(current_descriptor);
            heap_descriptor_clear_memory(current_descriptor);
            merge_descriptors(current_descriptor);

            if (current_descriptor->size % PAGE_SIZE == 0) {
                free_heap_page(current_descriptor->page);
                remove_heap_descriptor(current_descriptor);
                return;
            }
            return;
        }
    }
    print_heap();
    panic("Couldn't free memory!");
}