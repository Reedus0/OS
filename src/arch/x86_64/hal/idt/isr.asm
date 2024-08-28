global isr
extern irq_handler

section .text
bits 64

isr:
    push rax
    push rbx
    push rcx
    push rdx
    push rdi
    push rsi
    push rbp
    push rsp

    call irq_handler

    pop rsp
    pop rbp
    pop rsi
    pop rdi
    pop rdx
    pop rcx
    pop rbx
    pop rax

    add rsp, 16

    iretq

%include "src/arch/x86_64/hal/idt/isr_inc.asm"