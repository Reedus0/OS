global get_rax
global get_rbx
global get_rcx
global get_rdx
global get_rdi
global get_rsi
global get_rbp
global get_rsp
global get_rflags
global get_regs
global init_context
global switch_context
global get_stack
global __rdmsr
global __wrmsr

extern print_regs

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
get_rflags:
    push rbx

    pushfq
    pop rbx
    mov rax, rbx
    push rbx
    popfq

    pop rbx

    ret

get_regs:
    mov [rdi], rsp
    mov [rdi + 8], rbp
    mov [rdi + 16], rsi
    mov [rdi + 24], rdi
    mov [rdi + 32], rdx
    mov [rdi + 40], rcx
    mov [rdi + 48], rbx
    mov [rdi + 56], rax

    pushfq
    pop rax
    mov [rdi + 64], rax
    push rax
    popfq

    xor rax, rax

    ret 

init_context:
    mov [rdi], rsi
    ret

switch_context:
    mov r15, rsi
    
    mov rsp, [rdi]
    mov rbp, [rdi + 8]
    mov rsi, [rdi + 16]
    mov rdx, [rdi + 32]
    mov rcx, [rdi + 40]
    mov rbx, [rdi + 48]
    mov rax, [rdi + 56]

    mov rax, [rdi + 64]
    push rax
    popfq

    mov rdi, [rdi + 24]

    jmp r15

get_stack:
    push rbp
    mov rbp, rsp

    xor cl, cl
    mov rbx, rbp

    ret

__rdmsr:
    mov rcx, rdi
    xor rax, rax
    rdmsr

    shl rdx, 32
    or rax, rdx
    ret

__wrmsr:
    mov rcx, rdi
    mov eax, esi
    shr rsi, 32
    mov edx, esi
    wrmsr

    ret

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
