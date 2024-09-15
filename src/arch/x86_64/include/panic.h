#pragma once

#include "kernel/panic.h"
#include "kernel/printk.h"
#include "include/kalloc.h"
#include "include/types.h"
#include "asm/asm.h"

#define STACK_FRAMES_COUNT 7

void print_regs() {
    struct regs regs;

    get_regs(&regs);

    printk(NONE, "Registers:\n");
    printk(NONE, "rsp: 0x%16x rbp: 0x%16x\nrsi: 0x%16x rdi: 0x%16x\n",
     regs.rsp, regs.rbp, regs.rsi, regs.rdi);
    printk(NONE, "rdx: 0x%16x rcx: 0x%16x\nrbx: 0x%16x rax: 0x%16x\n",
     regs.rdx, regs.rcx, regs.rbx, regs.rax);
    printk(NONE, "rflags: 0x%16x\n", regs.rflags);
}

void print_stack() {
    struct stack_frame* stack = kalloc(sizeof(struct stack_frame) * STACK_FRAMES_COUNT);

    byte count = get_stack(stack, STACK_FRAMES_COUNT);
    printk(NONE, "Stack trace:\n");
    for (size_t i = 0; i < count; i++) {
        printk(NONE, "0x%16x\n", stack[i].rip - 5);
    }
    kfree(stack);
}