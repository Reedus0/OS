#pragma once

#include <stdint.h>
#include <stddef.h>

typedef size_t (*syscall)(size_t arg_1, size_t arg_2, size_t arg_3, size_t arg_4);

syscall g_syscalls[256];

void init_syscalls();