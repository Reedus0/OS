global long_mode_start
extern kmain

section .text
bits 64
long_mode_start:
    mov rax, 0
    mov rbx, 0
    mov rcx, 0
    mov rdx, 0
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call kmain

    hlt