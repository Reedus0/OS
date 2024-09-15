#pragma once

#include <stdint.h>
#include <stddef.h>

struct regs;

void get_regs(struct regs* regs);
void init_context(struct regs* regs, uint64_t sp);
void swith_context(struct regs* regs, uint64_t ip);