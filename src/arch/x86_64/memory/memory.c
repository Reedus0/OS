#include "kernel/io.h"
#include "memory.h"
#include "paging.h"
#include "heap.h"

shell_command sh_memory(char* command) {
    printk(NONE, "Total pages: %d\n", g_total_pages);
    printk(NONE, "Used: %d\n", g_total_pages - g_available_pages);
    printk(NONE, "Heap:\n");
    printk(NONE, "Descriptors: %d, Pages: %d\n", g_heap_descriptor_count, g_heap_pages_count);
    return 0;
}