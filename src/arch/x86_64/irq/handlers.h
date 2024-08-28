#pragma once

#include <stdint.h>
#include <stddef.h>

#include "include/regs.h"

struct irq_data {
    regs_t regs;
    uint64_t interrupt_number;
    uint64_t error_code;
    uint64_t original_rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t original_rsp;
    uint64_t ss;
} __attribute__((packed));
typedef struct irq_data irq_data_t;


typedef void (*interrupt)(irq_data_t* irq_data);

interrupt g_interrupt_handlers[256];

void __attribute__((cdecl)) irq_handler(irq_data_t);
void init_irq_handlers();