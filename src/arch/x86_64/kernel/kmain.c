#include <stdio.h>
#include "arch/x86_64/boot/multiboot2.h"
#include "arch/x86_64/hal/hal.h"
#include "kernel/shell.h"

void kmain (multiboot2_info_t* mbd) {
    init_hal(mbd);
    init_shell();
    while(1); 
}