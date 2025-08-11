#pragma once

#include "kernel/panic.h"
#include "kernel/io.h"
#include "kernel/elf.h"
#include "include/kalloc.h"
#include "include/types.h"
#include "asm/asm.h"

#define STACK_FRAMES_COUNT 7

void print_regs(struct regs* regs) {
    printk(NONE, "Registers:\n");
    printk(NONE, "rsp: 0x%16x rbp: 0x%16x\nrsi: 0x%16x rdi: 0x%16x\n",
        regs->rsp, regs->rbp, regs->rsi, regs->rdi);
    printk(NONE, "rdx: 0x%16x rcx: 0x%16x\nrbx: 0x%16x rax: 0x%16x\n",
        regs->rdx, regs->rcx, regs->rbx, regs->rax);
    printk(NONE, "r8:  0x%16x r9:  0x%16x\nr10: 0x%16x r11: 0x%16x\n",
        regs->r8, regs->r9, regs->r10, regs->r11);
    printk(NONE, "r12: 0x%16x r13: 0x%16x\nr14: 0x%16x r15: 0x%16x\n",
        regs->r12, regs->r13, regs->r14, regs->r15);
    printk(NONE, "rflags: 0x%8x\n", regs->rflags);
}

void print_stack() {
    struct stack_frame* stack = kalloc(sizeof(struct stack_frame) * STACK_FRAMES_COUNT);

    byte count = get_stack(stack, STACK_FRAMES_COUNT);
    printk(NONE, "Stack trace:\n");
    for (size_t i = 0; i < count; i++) {
        size_t call_address = stack[i].rip - 5;
        elf64_symbol_t* symbol = elf_get_symbol(g_kernel_elf, call_address);

        if (call_address == symbol->address) {
            printk(NONE, "0x%8x (%s)\n", call_address, symbol->name);
        }
        else {
            printk(NONE, "0x%8x (%s+0x%x)\n", call_address, symbol->name, call_address - symbol->address);
        }

    }
    kfree(stack);
}