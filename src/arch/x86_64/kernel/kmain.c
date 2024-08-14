#include <stdio.h>
#include "arch/x86_64/boot/multiboot2.h"
#include "arch/x86_64/hal/hal.h"
#include "include/kalloc.h"

void kmain (multiboot2_info_t* mbd) {
    init_hal(mbd);

    printf("Hello, Kernel!\n");

    while(1); 
}