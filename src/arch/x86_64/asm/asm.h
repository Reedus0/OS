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
    uint64_t r8;
    uint64_t r9;
    uint64_t r10;
    uint64_t r11;
    uint64_t r12;
    uint64_t r13;
    uint64_t r14;
    uint64_t r15;
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
uint16_t get_gs();
uint16_t get_fs();

void set_gs(uint16_t value);
void set_fs(uint16_t value);

uint64_t __rdmsr(uint64_t register);
void __wrmsr(uint64_t register, uint64_t value);

size_t get_context_register();
size_t get_stack(struct stack_frame* buffer, size_t max_frames);