#include "syscall.h"

size_t syscall(syscall_data_t* syscall_data) {
    syscall_data->current_task->status = NOT_READY;

    g_syscalls[syscall_data->number](syscall_data->arg_1, syscall_data->arg_2, syscall_data->arg_3, syscall_data->arg_4);

    syscall_data->current_task->status = READY;
    kfree(syscall_data);
    return 0;
}

void set_syscall_handler(size_t number, size_t handler) {
    g_syscalls[number] = handler;
}