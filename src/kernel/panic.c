#include "kernel/printk.h"
#include "panic.h"
#include "asm/io.h"

void panic(char* string) {
    struct regs regs;

    get_regs(&regs);

    disable_irq();
    print_regs(regs);
    print_stack();
    printk(ERROR, "Kernel panic: %s\n", string);
    while(1);
}