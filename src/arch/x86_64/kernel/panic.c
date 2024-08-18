#include "include/panic.h"
#include "arch/x86_64/asm/io.h"

void panic(char* string) {
    disable_irq();
    printk("Kernel panic: %s\n", string);
    while(1);
}