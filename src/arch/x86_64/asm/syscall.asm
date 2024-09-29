extern __syscall

bits 64
section .text
__syscall:
    mov rax, rdi
    mov rdi, rsi
    mov rsi, rdx
    mov rdx, rcx
    mov rcx, r8
    int 80h
    ret