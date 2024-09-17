#pragma once

#include <stdint.h>
#include <stddef.h>

struct regs;

void set_regs(struct regs* regs);
void run_context(struct regs* regs);
void init_context(struct regs* regs, uint64_t sp);
void save_ip(uint64_t value);
uint64_t restore_ip();

uint16_t get_task_register();
void set_task_register(uint16_t value);
uint16_t get_old_task_register();
void set_old_task_register(uint16_t value);