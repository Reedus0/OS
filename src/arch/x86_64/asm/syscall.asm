global __syscall
global register_syscall

extern __wrmsr

section .text
bits 64

__syscall:
    xor rax, rax
    xor rbx, rbx

    mov ax, 1 << 3
    mov bx, 3 << 3

    shl rbx, 16
    or rax, rbx
    shl rax, 32

    mov rdi, 0xC0000081
    mov rsi, rax
    call __wrmsr

    mov rdi, 0xC0000082
    mov rsi, rdi
    call __wrmsr

    syscall
    retn