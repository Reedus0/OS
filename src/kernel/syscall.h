#pragma once

#include <stdint.h>
#include <stddef.h>

typedef size_t (*syscall)(size_t arg_1, ...);

syscall g_syscalls[256];

void init_syscalls();