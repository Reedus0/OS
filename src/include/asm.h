#pragma once

#include <stdint.h>
#include <stddef.h>

struct regs;

void get_regs(struct regs* regs);
void init_context(struct regs* regs, uint64_t sp);
void swith_context(struct regs* regs, uint64_t ip);
uint64_t get_task_register();
void set_task_register(uint64_t value);
uint64_t get_old_task_register();
void set_old_task_register();