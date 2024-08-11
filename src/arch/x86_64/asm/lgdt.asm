global load_gdt

section .text
bits 32

load_gdt:
    lgdt [edi]

    ret