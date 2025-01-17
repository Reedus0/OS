extern __syscall

bits 64
section .text
__syscall:
    mov rax, rdi
    int 80h
    ret