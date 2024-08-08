global inb
global outb
global io_wait
global enable_irq
global disable_irq

section .text
bits 64

inb:
    mov dx, di
    in al, dx
    ret

outb:
    mov dx, di
    mov al, sil
    out dx, al
    ret

io_wait:
    mov di, 0x80
    mov si, 0

    call outb

    ret

enable_irq:
    sti
    ret

disable_irq:
    cli
    ret