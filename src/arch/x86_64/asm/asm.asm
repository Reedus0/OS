global get_rax
global get_rbx
global get_rcx
global get_rdx
global get_rdi
global get_rsi
global get_rbp
global get_rsp
global get_rflags
global get_gs
global get_fs
global get_context_register

global get_regs
global set_regs

global init_context

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
get_context_register:
    mov rax, r15
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

    mov [rdi + 72], r8
    mov [rdi + 80], r9
    mov [rdi + 88], r10
    mov [rdi + 96], r11
    mov [rdi + 104], r12
    mov [rdi + 112], r13
    mov [rdi + 120], r14
    mov [rdi + 128], r15

    xor rax, rax

    ret 

set_regs:
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

    mov r8, [rdi + 72]
    mov r9, [rdi + 80]
    mov r10, [rdi + 88]
    mov r11, [rdi + 96]
    mov r12, [rdi + 104]
    mov r13, [rdi + 112]
    mov r14, [rdi + 120]
    mov r15, [rdi + 128]

    mov rdi, [rdi + 24]

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

init_context:
    mov [rdi + 128], rsi
    mov [rdi], rdx
    mov [rdi + 24], rcx
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

    cmp rdx, rbx
    jbe .done

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