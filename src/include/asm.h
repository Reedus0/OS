#pragma once

#include <stdint.h>
#include <stddef.h>

struct regs;

void print_regs(struct regs regs);

void get_regs(struct regs* regs);
void set_regs(struct regs* regs);
void init_context(struct regs* regs, size_t sp);