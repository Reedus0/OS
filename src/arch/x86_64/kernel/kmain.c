#include "boot/multiboot2.h"
#include "hal/hal.h"
#include "memory/memory.h"
#include "drivers/bus/pci/pci.h"
#include "kernel/shell.h"
#include "kernel/utils.h"

void kmain(multiboot2_info_t* mbd) {
    init_hal(mbd);

    static shell_function_t functions[] = {
        {NULL, NULL},
        {"hello", sh_hello},
        {"clear", sh_clear},
        {"module", sh_module},
        {"pci", sh_pci},
        {"memory", sh_memory},
        {NULL, NULL},
    };
    init_shell(functions);
    while(1);
}