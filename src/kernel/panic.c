#include "kernel/io.h"
#include "panic.h"
#include "asm/io.h"

void panic(char* string) {
    disable_irq();

    struct regs regs;
    get_regs(&regs);

    print_regs(&regs);
    print_stack();

    printk(ERROR, "Kernel panic: %s\n", string);
    while (1);
}