#include <stdio.h>
#include "arch/x86_64/hal/hal.h"
#include "include/print.h"

void kmain () {

    init_hal();

    print_clear();
    printf("Hello, Kernel!\n");

    while(1); 
}