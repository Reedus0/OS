#include <stdio.h>
#include "arch/x86_64/boot/multiboot2.h"
#include "arch/x86_64/hal/hal.h"
#include "include/print.h"

void kmain (multiboot2_info_t* mbd) {
    init_hal(mbd);

    printf("Hello, Kernel!\n");

    int* ptr = 0x7fffe01233;
    *ptr = 1;
    printf("%x", *ptr);
    while(1); 
}