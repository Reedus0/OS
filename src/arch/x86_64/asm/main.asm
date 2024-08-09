global start
global gdt

extern long_mode_start

section .text
bits 32
start:
    mov esp, stack_bottom
    
    call check_cpuid
    call check_long_mode

    call setup_pages
    call enable_paging

    lgdt [gdt.pointer]
    jmp gdt.code_segment:long_mode_start

check_cpuid:
    pushfd
    pop eax
    mov ecx, eax
    xor eax, 1 << 21
    push eax
    popfd
    pushfd
    pop eax
    push ecx
    popfd
    cmp eax, ecx
    je stop

    ret

check_long_mode:
    push ebx
    mov eax, 0x80000000
    cpuid
    cmp eax, 0x80000001
    jb stop

    mov eax, 0x80000001
    cpuid
    test edx, 1 << 29
    jz stop
    pop ebx

    ret

setup_pages:
    mov eax, page_table_l3
    or eax, 0b11
    mov [page_table_l4], eax

    mov eax, page_table_l2
    or eax, 0b11
    mov [page_table_l3], eax

    mov ecx, 0
    
.loop:
    mov eax, 0x200000
    mul ecx
    or eax, 0b10000011
    mov [page_table_l2 + ecx * 8], eax

    inc ecx
    cmp ecx, 512
    jne .loop

    ret
    
enable_paging:
    mov eax, page_table_l4
    mov cr3, eax

    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    mov ecx, 0xc0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax

    ret
stop:
    hlt
section .bss
align 4096
page_table_l4:
    resb 4096
page_table_l3:
    resb 4096
page_table_l2:
    resb 4096
stack_top:
    resb 4096 * 4
stack_bottom:

section .rodata
gdt:
    dq 0
.code_segment: equ $ - gdt
    dq (1 << 43) | (1 << 44) | (1 << 47) | (1 << 53)
.pointer:
    dw $ - gdt - 1
    dq gdt