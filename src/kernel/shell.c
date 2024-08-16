#include "shell.h"
#include "include/print.h"

#include "kernel/shell/hello.h"
#include "kernel/shell/clear.h"
#include "kernel/shell/driver.h"

static size_t (*shell_commnads[])() = {
    NULL, sh_hello, sh_clear, sh_driver
};

static char* command_names[] = {
    NULL,
    "hello",
    "clear",
    "driver"
};

static size_t search_command(char* command) {
    for (size_t i = 1; i < sizeof(shell_commnads) / sizeof(shell_commnads[0]); i++) {
        char* current_command = command_names[i];
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
    }
    return 0;
}

static void clear_shell_buffer() {
    for (size_t i = 0; i < SHELL_BUFFER_SIZE; i++) {
        g_shell_buffer[i] = 0;
    }
}

void init_shell() {
    print_clear();

    while (1) {
        printf("%s", g_prompt);
        scanf("%s", g_shell_buffer);
        shell_execute(g_shell_buffer);
        clear_shell_buffer();
    }
}

size_t shell_execute(char* command) {
    size_t index = search_command(command);
    if (index == 0) {
        printf("Unknown command!\n");
        return 1;
    }
    return shell_commnads[index](command);
}