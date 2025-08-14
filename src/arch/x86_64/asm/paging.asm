global get_page_fault_address
global flush_page
global set_page_directory
global get_page_directory

extern g_kernel_table_l4

section .text
bits 64
get_page_fault_address:
    mov rax, cr2

    ret

flush_page:
    mov rax, cr3
    mov cr3, rax

    ret

set_page_directory:
    mov cr3, rdi

    ret

get_page_directory:
    mov rax, cr3

    ret