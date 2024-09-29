#include "syscall.h"
#include "kernel/printk.h"

syscall test_call(size_t arg_1, size_t arg_2, size_t arg_3, size_t arg_4) {
    return 0x1234;
}

static void set_syscall_handler(size_t number, size_t handler) {
    g_syscalls[number] = handler;
}

void init_syscalls() {
    set_syscall_handler(0, test_call);
}