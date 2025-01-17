#pragma once

#include <stdint.h>
#include <stddef.h>

typedef size_t (*syscall_t)(size_t arg_1, ...);

syscall_t g_syscalls[256];

size_t syscall(size_t number, size_t arg_1, size_t arg_2, size_t arg_3, size_t arg_4);
void init_syscalls();