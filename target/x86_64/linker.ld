ENTRY(start)

SECTIONS {
    . = 1M;
    
    .boot : {
        KEEP(*(.multiboot_header))
    }

    .text : {
        *(.text)
    }
    . = 128M;
    .bss : {
        *(.bss)
    }
}