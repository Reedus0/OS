global io_in
global io_out
global enable_irq
global deisable_irq
global crash

section .text
bits 64

io_in:
    mov dx, di
    in ax, dx
    ret

io_out:
    mov dx, di
    mov ax, si
    out dx, ax
    ret

enable_irq:
    sti
    ret

deisable_irq:
    cli
    ret

crash:
    mov ax, 6
    mov bx, 0
    div bx

    ret