global isr
extern irq_handler
extern get_task_register
extern get_task

section .text
bits 64

isr:
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

    pop rax
    pop rbx
    pop rcx
    pop rdx
    pop rdi
    pop rsi
    pop rbp
    add rsp, 8
    popfq

    add rsp, 16
    iretq

%include "src/arch/x86_64/irq/isr_inc.asm"