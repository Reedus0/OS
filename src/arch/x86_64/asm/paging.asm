global g_phisycal_pages
global g_page_table_l4
global g_page_table_l3
global g_page_table_l2

global get_page_fault_address
global flush_page

section .text
bits 64
get_page_fault_address:
    mov rax, cr2

    ret

flush_page:
    mov rax, g_page_table_l4
    mov cr3, rax

    ret

section .bss
align 4096
g_phisycal_pages:
    resb 65536
g_page_table_l4:
    resb 4096
g_page_table_l3:
    resb 4096
g_page_table_l2:
    resb 4096 * 512