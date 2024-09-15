#pragma once

#include <stdint.h>
#include <stddef.h>

#include "include/asm.h"

struct regs {
    uint64_t rsp;
    uint64_t rbp;
    uint64_t rsi;
    uint64_t rdi;
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rbx;
    uint64_t rax;
    uint64_t rflags;
} __attribute__((packed));

struct stack_frame {
    struct stack_frame* rbp;
    uint64_t rip;
} __attribute__((packed));

uint64_t get_rax();
uint64_t get_rbx();
uint64_t get_rcx();
uint64_t get_rdx();
uint64_t get_rdi();
uint64_t get_rsi();
uint64_t get_rbp();
uint64_t get_rsp();
uint64_t get_rflags();

void get_regs(struct regs* regs);
void init_context(struct regs* regs, uint64_t sp);
void swith_context(struct regs* regs, uint64_t ip);

uint64_t __rdmsr(uint64_t register);
void __wrmsr(uint64_t register, uint64_t value);

size_t get_stack(struct stack_frame* buffer, size_t max_frames);