global get_page_fault_address
global flush_page

extern g_page_table_l4

section .text
bits 64
get_page_fault_address:
    mov rax, cr2

    ret

flush_page:
    mov rax, g_page_table_l4
    mov cr3, rax

    ret