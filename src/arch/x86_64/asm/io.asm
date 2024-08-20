global in8
global out8
global in16
global out16
global in32
global out32
global io_wait
global enable_irq
global disable_irq

section .text
bits 64

in8:
    mov dx, di
    in al, dx
    ret

out8:
    mov dx, di
    mov al, sil
    out dx, al
    ret

in16:
    mov dx, di
    mov rdi, io_buffer
    insw
    mov eax, [io_buffer]
    ret

out16:
    mov dx, di
    mov [io_buffer], rsi
    mov rsi, io_buffer
    outsw
    ret

in32:
    mov dx, di
    mov rdi, io_buffer
    insd
    mov eax, [io_buffer]
    ret

out32:
    mov dx, di
    mov [io_buffer], rsi
    mov rsi, io_buffer
    outsd
    ret

io_wait:
    mov di, 0x80
    mov si, 0

    call out8

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