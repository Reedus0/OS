#include "include/panic.h"
#include "asm/io.h"

void panic(char* string) {
    disable_irq();
    printk("Kernel panic: %s\n", string);
    while(1);
}