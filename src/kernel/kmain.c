#include "arch/x86_64/io.h"
#include "include/print.h"

void kmain () {
    char* string = "Hello, Kernel!\n";
    print_clear();
    print_string(string);

    setup_gdt();

    setup_idt();
    setup_irq_handlers();

    enable_irq();
    crash();
    while(1); 
}