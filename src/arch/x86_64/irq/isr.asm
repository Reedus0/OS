global isr
extern irq_handler
extern get_task_register
extern get_task

section .text
bits 64

isr:
    push r15
    push r14
    push r13
    push r12
    push r11
    push r10
    push r9
    push r8
    pushfq
    push rsp
    push rbp
    push rsi
    push rdi
    push rdx
    push rcx
    push rbx
    push rax

    call irq_handler

    add rsp, 8
    pop rbp
    pop rsi
    pop rdi
    pop rdx
    pop rcx
    pop rbx
    pop rax
    popfq
    pop r8
    pop r9
    pop r10
    pop r11
    pop r12
    pop r13
    pop r14
    pop r15

    add rsp, 16

    iretq

%include "src/arch/x86_64/irq/isr_inc.asm"