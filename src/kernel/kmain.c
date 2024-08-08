#include "arch/x86_64/io.h"
#include "include/print.h"

void kmain () {
    print_clear();
    print_string("Hello, Kernel!\n");

    init_gdt();

    init_idt();
    init_irq_handlers();

    init_pic();

    enable_irq();
    print_string("IRQ OK\n");

    while(1); 
}