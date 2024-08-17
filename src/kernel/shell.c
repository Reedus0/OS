#include "shell.h"
#include "include/print.h"

static size_t search_command(char* command, char* command_names[], size_t (*shell_commnads[])()) {
    size_t i = 1;
    char* current_command = command_names[i];
    while (current_command != NULL) {
        current_command = command_names[i];
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

static size_t shell_execute(char* command, char* command_names[], size_t (*shell_commnads[])()) {
    size_t index = search_command(command, command_names, shell_commnads);
    if (index == 0) {
        printf("Unknown command!\n");
        return 1;
    }
    return shell_commnads[index](command);
}

void init_shell(char* command_names[], size_t (*shell_commnads[])()) {
    print_clear();

    while (1) {
        printf("%s", g_prompt);
        scanf("%s", g_shell_buffer);
        shell_execute(g_shell_buffer, command_names, shell_commnads);
        clear_shell_buffer();
    }
}