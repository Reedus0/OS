#pragma once

#include <stdint.h>
#include <stddef.h>

#define SHELL_BUFFER_SIZE 256

char g_prompt[] = "OS>";
char g_shell_buffer[SHELL_BUFFER_SIZE];

typedef size_t status;
typedef status (*shell_command)();

void init_shell();
size_t shell_execute(char* command);