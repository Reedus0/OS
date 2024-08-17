#pragma once

#include <stdint.h>
#include <stddef.h>

#define SHELL_BUFFER_SIZE 256

char g_prompt[] = "OS>";
char g_shell_buffer[SHELL_BUFFER_SIZE];

typedef size_t status;
typedef status (*shell_command)(char* command);

void init_shell(char* command_names[], size_t (*shell_commnads[])());