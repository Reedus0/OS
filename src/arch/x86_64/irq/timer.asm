extern int_timer

bits 64
section .text

int_timer:
    mov eax, 1

    iretq