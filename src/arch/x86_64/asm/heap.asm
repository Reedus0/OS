global g_heap_descriptors
global g_heap_base

section .bss
g_heap_base:
    resb 0x2000000
g_heap_descriptors:
    resb 0x200000