#include "syscall.h"
#include "kernel/printk.h"
#include "kernel/sysout.h"
#include "kernel/syscall.h"
#include "asm/io.h"

syscall_t in(byte* buffer, size_t count) {
    task_t* current_task = get_task(g_current_task_id);

    clear_stream(&current_task->stdin);
    size_t last_size = 0;

    while (1) {
        while (!current_task->stdin_updated);
        current_task->stdin_updated = 0;
        byte last_byte = stream_get_last_byte(&current_task->stdin);
        size_t stdin_size = stream_get_size(&current_task->stdin);

        if (stdin_size > last_size) {
            sysout_add_byte(last_byte);
        }
        if (last_byte == '\n') {
            break;
        }
        if (stdin_size <= last_size) {
            if (last_size > 0) {
                sysout_add_byte('\b');
                last_size--;
            }
        }
        if (last_size < count) {
            buffer[last_size] = last_byte;
        }
        last_size = stdin_size;
    }
    return 0;
}

syscall_t out(byte* buffer, size_t count) {
    for (size_t i = 0; i < count; i++) {
        sysout_add_byte(buffer[i]);
    }
    return 0;
}

#define SYSCALL_IN 0
#define SYSCALL_OUT 1

void init_syscalls() {
    set_syscall_handler(SYSCALL_IN, in);
    set_syscall_handler(SYSCALL_OUT, out);
}