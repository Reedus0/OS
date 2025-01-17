#pragma once

#include <stdint.h>
#include <stddef.h>

#include "include/timer.h"
#include "include/asm.h"
#include "asm/asm.h"

struct irq_data {
    uint64_t interrupt_number;
    uint64_t error_code;
    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
} __attribute__((packed));
typedef struct irq_data irq_data_t;

typedef void (*interrupt_t)(irq_data_t* irq_data);

#define TIMER_FUNCTION(tick, func) if (g_ticks % tick == 0) func

interrupt_t g_interrupt_handlers[256];

void __attribute__((cdecl)) irq_handler(struct regs regs, irq_data_t irq_data);
void init_irq_handlers();