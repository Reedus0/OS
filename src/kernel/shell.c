#include "shell.h"
#include "include/print.h"
#include "kernel/kget.h"

static size_t search_command(char* command, shell_function_t functions[]) {
    size_t i = 1;
    char* current_command = functions[i].name;
    while (current_command != NULL) {
        current_command = functions[i].name;
        char* user_command = command;
        while(1) {
            if (*current_command == '\0') {
                return i;
            }
            if (*current_command != *user_command) {
                break;
            }
            current_command++;
            user_command++;
        }
        i++;
    }
    return 0;
}

static void clear_shell_buffer() {
    for (size_t i = 0; i < SHELL_BUFFER_SIZE; i++) {
        g_shell_buffer[i] = 0;
    }
}

static size_t shell_execute(char* command, shell_function_t functions[]) {
    size_t index = search_command(command, functions);
    if (index == 0) {
        printk("Unknown command!\n");
        return 1;
    }
    return functions[index].shell_commnad(command);
}

void init_shell(shell_function_t functions[]) {
    print_clear();
    g_shell_path = &g_vfs_root;
    while (1) {
        printk(g_prompt, g_shell_path->name);
        kget(g_shell_buffer);
        shell_execute(g_shell_buffer, functions);
        clear_shell_buffer();
    }
}