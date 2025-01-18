#pragma once

#include <stdint.h>
#include <stddef.h>

#include "include/task.h"

typedef size_t (*syscall_t)(size_t arg_1, ...);

syscall_t g_syscalls[256];

struct syscall_data {
    task_t* current_task;
    size_t number;
    size_t arg_1;
    size_t arg_2;
    size_t arg_3;
    size_t arg_4;
};
typedef struct syscall_data syscall_data_t;

size_t syscall(syscall_data_t* syscall_data);
void init_syscalls();

#define SYSCALL_IN 0
#define SYSCALL_OUT 1