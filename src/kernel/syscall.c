#include "syscall.h"
#include "kernel/printk.h"
#include "kernel/stdin.h"
#include "kernel/stdout.h"
#include "asm/io.h"

syscall_t in(byte* buffer, size_t count) {
    clear_stdin();
    size_t last_size = 0;
    while(1) {
        byte last_byte = stdin_get_last_byte();
        size_t stdin_size = stdin_get_size();
        
        if (stdin_size > last_size) {
            stdout_add_byte(last_byte);
        }
        if (last_byte == '\n') {
            break;
        }
        if (stdin_size <= last_size) {
            if (last_size > 0) {
                stdout_add_byte('\b');
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
    for (int i = 0; i < count; i++) {
        stdout_add_byte(buffer[i]);
    }
    return 0;
}

size_t syscall(syscall_data_t* syscall_data) {
    syscall_data->current_task->status = NOT_READY;

    g_syscalls[syscall_data->number](syscall_data->arg_1, syscall_data->arg_2, syscall_data->arg_3, syscall_data->arg_4);

    syscall_data->current_task->status = READY;
    kfree(syscall_data);
    return 0;
}

static void set_syscall_handler(size_t number, size_t handler) {
    g_syscalls[number] = handler;
}

void init_syscalls() {
    set_syscall_handler(SYSCALL_IN, in);
    set_syscall_handler(SYSCALL_OUT, out);
}