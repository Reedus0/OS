#include <stdio.h>
#include "arch/x86_64/boot/multiboot2.h"
#include "arch/x86_64/hal/hal.h"
#include "include/kalloc.h"

void kmain (multiboot2_info_t* mbd) {
    init_hal(mbd);

    printf("Hello, Kernel!\n");

    void* ptr = kalloc(16);

    printf("%p\n", ptr);
    kfree(ptr);
    void* ptr2 = kalloc(8);
    printf("%p\n", ptr2);

    void* ptr3 = kalloc(8);
    printf("%p\n", ptr3);

    void* ptr4 = kalloc(8);
    printf("%p\n", ptr4);

    kfree(ptr3);

    void* ptr5 = kalloc(86);
    printf("%p\n", ptr5);
    
    void* ptr6 = kalloc(8);
    printf("%p\n", ptr6);

    void* ptr7 = kalloc(816);
    printf("%p\n", ptr7);

    while(1); 
}