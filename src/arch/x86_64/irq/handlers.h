#pragma once

#include <stdint.h>
#include <stddef.h>

#include "include/regs.h"

struct irq_data {
    struct regs regs;
    uint64_t interrupt_number;
    uint64_t error_code;
    uint64_t original_rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t original_rsp;
    uint64_t ss;
} __attribute__((packed));


typedef void (*interrupt)(struct irq_data* irq_data);

interrupt g_interrupt_handlers[256];

void __attribute__((cdecl)) irq_handler(irq_data);
void init_irq_handlers();