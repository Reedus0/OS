#pragma once

#include "kernel/panic.h"
#include "include/kalloc.h"
#include "include/types.h"
#include "asm/asm.h"

#define STACK_FRAMES_COUNT 7

void print_regs() {
    struct regs* regs = get_regs();

    printk("Registers:\n");
    printk("rsp: 0x%x\nrbp: 0x%x\nrsi: 0x%x\nrdi: 0x%x\n",
     regs->rsp, regs->rbp, regs->rsi, regs->rdi);
    printk("rdx: 0x%x\nrcx: 0x%x\nrbx: 0x%x\nrax: 0x%x\n",
     regs->rdx, regs->rcx, regs->rbx, regs->rax);
}

void print_stack() {
    struct stack_frame* stack = kalloc(sizeof(struct stack_frame) * STACK_FRAMES_COUNT);

    byte count = get_stack(stack, STACK_FRAMES_COUNT);
    printk("Stack trace:\n");
    for (size_t i = 0; i < count; i++) {
        printk("func: %x\n", stack[i].rip - 5);
    }
    kfree(stack);
}