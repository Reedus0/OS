global load_idt

section .text
bits 32

load_idt:
    lidt [edi]

    ret