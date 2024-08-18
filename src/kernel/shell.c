#include "shell.h"
#include "include/print.h"

static size_t search_command(char* command, shell_function_t functions[]) {
    size_t i = 1;
    char* current_command = functions[i].name;
    while (current_command != NULL) {
        current_command = functions[i].name;
        while(1) {
            if (*current_command == '\0') {
                return i;
            }
            if (*current_command != *command) {
                break;
            }
            current_command++;
            command++;
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

    while (1) {
        printk("%s", g_prompt);
        scank("%s", g_shell_buffer);
        shell_execute(g_shell_buffer, functions);
        clear_shell_buffer();
    }
}