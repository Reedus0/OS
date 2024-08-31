#include "shell.h"
#include "include/print.h"
#include "kernel/kget.h"
#include "lib/string.h"

static size_t search_command(char* command, shell_function_t functions[]) {
    size_t i = 1;
    char* current_command = functions[i].name;
    while (current_command != NULL) {
        current_command = functions[i].name;
        if (strncmp(command, current_command, strlen(current_command))) {
            return i;
        }
        i++;
    }
    return 0;
}

static size_t shell_execute(char* command, shell_function_t functions[]) {
    size_t index = search_command(command, functions);
    if (index == 0) {
        printk("Unknown command!\n");
        return 1;
    }
    return functions[index].shell_commnad(command);
}

static void clear_shell_buffer() {
    for (size_t i = 0; i < SHELL_BUFFER_SIZE; i++) {
        g_shell_buffer[i] = 0;
    }
}

void init_shell(shell_function_t functions[]) {
    print_clear();
    g_shell_dir = &g_vfs_root;
    while (1) {
        printk(g_prompt, g_shell_dir->name);
        kget(g_shell_buffer);
        shell_execute(g_shell_buffer, functions);
        clear_shell_buffer();
    }
}