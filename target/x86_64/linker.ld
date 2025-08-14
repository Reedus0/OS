ENTRY(start)

SECTIONS {
    . = 1M;
    
    .boot : {
        KEEP(*(.multiboot_header))
    }

    . = 16M;

    .text : {
        *(.text)
    }
    . = 128M;
    .bss : {
        *(.bss)
    }
}