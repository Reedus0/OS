#include "arch/x86_64/hal/hal.h"
#include "include/print.h"

void kmain () {
    print_clear();
    print_string("Hello, Kernel!\n");

    init_hal();

    while(1); 
}