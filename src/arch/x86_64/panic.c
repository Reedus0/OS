#include <stdio.h>
#include "include/panic.h"
#include "arch/x86_64/io.h"

void panic() {
    disable_irq();
    printf("Kernel panic!!!\n");
    while(1);
}