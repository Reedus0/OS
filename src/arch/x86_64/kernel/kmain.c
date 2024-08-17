#include <stdio.h>
#include "arch/x86_64/boot/multiboot2.h"
#include "arch/x86_64/hal/hal.h"
#include "arch/x86_64/drivers/bus/pci/pci.h"
#include "arch/x86_64/memory/memory.h"
#include "kernel/shell.h"
#include "kernel/utils.h"

void kmain (multiboot2_info_t* mbd) {
    init_hal(mbd);
    static size_t (*shell_commnads[])() = {
    NULL, sh_hello, sh_clear, sh_driver, sh_pci, sh_memory, NULL
    };

    static char* command_names[] = {
        NULL,
        "hello",
        "clear",
        "driver",
        "pci",
        "memory",
        NULL
    };

    init_shell(command_names, shell_commnads);
    while(1); 
}