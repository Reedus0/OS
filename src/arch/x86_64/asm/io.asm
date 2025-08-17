global in8
global out8
global in16
global out16
global in32
global out32
global io_wait
global enable_irq
global disable_irq

extern g_io_buffer

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

    mov rax, 0xFFFF800000000000
    lea rsi, [rax + g_io_buffer]

    mov rdi, rsi

    insw

    xor rax, rax
    mov eax, [rsi]
    ret

out16:
    mov dx, di

    mov rax, 0xFFFF800000000000
    lea rax, [rax + g_io_buffer]

    mov [rax], rsi
    mov rsi, rax

    outsw

    ret

in32:
    mov dx, di

    mov rax, 0xFFFF800000000000
    lea rsi, [rax + g_io_buffer]

    mov rdi, rsi

    insd

    xor rax, rax
    mov eax, [rsi]
    ret

out32:
    mov dx, di

    mov rax, 0xFFFF800000000000
    lea rax, [rax + g_io_buffer]

    mov [rax], rsi
    mov rsi, rax

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