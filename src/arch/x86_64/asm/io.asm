global inb
global outb
global ind
global outd
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

ind:
    mov dx, di
    mov rdi, io_buffer
    insd
    mov eax, [io_buffer]
    ret

outd:
    mov dx, di
    mov [io_buffer], rsi
    mov rsi, io_buffer
    outsd
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

section .bss
io_buffer:
    dd 0