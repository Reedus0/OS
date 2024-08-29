global isr
extern irq_handler

section .text
bits 64

isr:
    call irq_handler

    add rsp, 16

    iretq

%include "src/arch/x86_64/hal/idt/isr_inc.asm"