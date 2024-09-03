#include "panic.h"
#include "asm/io.h"

void panic(char* string) {
    disable_irq();
    // print_regs();
    // print_stack();
    printk("Kernel panic: %s\n", string);
    while(1);
}