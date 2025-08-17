#include "shell.h"
#include "drivers/tty/tty.h"
#include "kernel/io.h"
#include "include/task.h"
#include "lib/string.h"
#include "asm/asm.h"
#include "include/asm.h"
#include "memory/paging.h"

static size_t search_command(char* command, shell_function_t functions[]) {
    size_t i = 1;
    char* current_command = get_virtual_address(functions[i].name);
    while (current_command != get_virtual_address(NULL)) {
        if (strncmp(command, current_command, strlen(current_command))) {
            return i - 1;
        }
        current_command = get_virtual_address(functions[i].name);
        i++;
    }
    return 0;
}

static size_t shell_execute(char* command, shell_function_t functions[]) {
    size_t index = search_command(command, functions);
    if (index == 0) {
        printk(NONE, "Unknown command!\n");
        return 1;
    }
    size_t(*shell_command)() = (size_t)get_virtual_address(functions[index].shell_command);
    return shell_command(command);
}

static void clear_shell_buffer() {
    for (size_t i = 0; i < SHELL_BUFFER_SIZE; i++) {
        g_shell_buffer[i] = 0;
    }
}

void init_shell(shell_function_t functions[]) {
    MODULE_FUNCTION(g_terminal->driver, TTY_CLEAR)(g_terminal);
    g_shell_dir = &g_vfs_root;
    while (1) {
        printk(NONE, g_prompt, g_shell_dir->name);
        kget(g_shell_buffer, SHELL_BUFFER_SIZE);
        shell_execute(g_shell_buffer, functions);
        clear_shell_buffer();
    }
}