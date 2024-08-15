#include <stdio.h>
#include "arch/x86_64/boot/multiboot2.h"
#include "arch/x86_64/hal/hal.h"
#include "include/kalloc.h"

void kmain (multiboot2_info_t* mbd) {
    init_hal(mbd);

    printf("Hello, Kernel!\n");

    int* ptr = 0x40000000;
    *ptr = 1;
    printf("%x", *ptr);

    int* ptr2 = 0x80000000;
    *ptr2 = 2;
    printf("%x", *ptr2);

    int* ptr3 = 0x40000000;
    *ptr3 = 1;
    printf("%x", *ptr3);

    while(1); 
}