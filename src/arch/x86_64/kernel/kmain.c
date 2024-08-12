#include <stdio.h>
#include "arch/x86_64/boot/multiboot2.h"
#include "arch/x86_64/hal/hal.h"
#include "include/print.h"

void kmain (multiboot2_info_t* mbd) {
    init_hal(mbd);

    printf("Hello, Kernel!\n");

    size_t* ptr = 0x80000000;
    *ptr = 1;
    printf("%x\n", *ptr);
    size_t* ptr2 = 0x40000000;
    *ptr2 = 1;
    printf("%x\n", *ptr2);
    size_t* ptr3 = 0x7ffffff00;
    *ptr3 = 1;
    printf("%x\n", *ptr3);
    while(1); 
}