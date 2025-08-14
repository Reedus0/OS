global long_mode_start
extern g_kernel_table_l4

extern kmain

section .text
bits 64
long_mode_start:

    mov rax, 0
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov rbp, rsp
    mov rax, 0xFFFF800000000000
    lea rsp, [rbp + rax]

    xor rbp, rbp

    mov rax, 0xFFFF800000000000
    lea rdi, [rdi + rax]

    mov rax, 0xFFFF800000000000
    lea rax, [rax + kmain]

    jmp rax
    
    hlt