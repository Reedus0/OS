#include "arch/x86_64/boot/multiboot2.h"
#include "arch/x86_64/hal/hal.h"
#include "arch/x86_64/drivers/bus/pci/pci.h"
#include "arch/x86_64/memory/memory.h"
#include "kernel/shell.h"
#include "kernel/utils.h"

void kmain(multiboot2_info_t* mbd) {
    init_hal(mbd);

    static shell_function_t functions[] = {
        {NULL, NULL},
        {"hello", sh_hello},
        {"clear", sh_clear},
        {"driver", sh_driver},
        {"pci", sh_pci},
        {"memory", sh_memory},
        {NULL, NULL},
    };
    
    init_shell(functions);
    while(1);
}