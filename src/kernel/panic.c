#include "include/print.h"
#include "panic.h"

void panic(struct regs regs) {
    print_string("Kernel panic!!!");
    print_string("\nrsp: ");
    print_number(regs.rsp);
    print_string("\nrbp: ");
    print_number(regs.rbp);
    print_string("\nrsi: ");
    print_number(regs.rsi);
    print_string("\nrdi: ");
    print_number(regs.rdi);
    print_string("\nrdx: ");
    print_number(regs.rdx);
    print_string("\nrcx: ");
    print_number(regs.rcx);
    print_string("\nrbx: ");
    print_number(regs.rbx);
    print_string("\nrax: ");
    print_number(regs.rax);
    while(1);
}