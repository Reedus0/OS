global get_rax
global get_rbx
global get_rcx
global get_rdx
global get_rdi
global get_rsi
global get_rbp
global get_rsp
global get_regs
global get_stack

extern g_regs

section .text
bits 64

get_rax:
    ret
get_rbx:
    mov rax, rbx
    ret
get_rcx:
    mov rax, rcx
    ret
get_rdx:
    mov rax, rdx
    ret
get_rdi:
    mov rax, rdi
    ret
get_rsi:
    mov rax, rsi
    ret
get_rbp:
    mov rax, rbp
    ret
get_rsp:
    mov rax, rsp
    ret
get_regs:
    mov [g_regs], rsp
    mov [g_regs + 8], rbp
    mov [g_regs + 16], rsi
    mov [g_regs + 24], rdi
    mov [g_regs + 32], rdx
    mov [g_regs + 40], rcx
    mov [g_regs + 48], rbx
    mov [g_regs + 56], rax

    mov rax, g_regs

    ret 

get_stack:
    push rbp
    mov rbp, rsp

    xor cl, cl
    mov rbx, rbp

.loop:

    cmp cl, sil
    je .done
    inc cl

    mov rdx, [rbx]
    mov [rdi], rdx
    mov rdx, [rbx + 8]
    mov [rdi + 8], rdx
    mov rbx, [rbx]
    add rdi, 16

    cmp rbx, 0
    je .done
   
    jmp .loop

.done:
    mov al, cl

    mov rsp, rbp
    pop rbp
    ret
