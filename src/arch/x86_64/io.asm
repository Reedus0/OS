global io_in
global io_out

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