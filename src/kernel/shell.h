#pragma once

#include <stdint.h>
#include <stddef.h>

#include "fs/vfs.h"

#define SHELL_PATH_SIZE 256
#define SHELL_BUFFER_SIZE 256

char g_prompt[] = "OS:%s>";
char g_shell_buffer[SHELL_BUFFER_SIZE];

dir_t* g_shell_dir;

struct shell_function {
    char* name;
    size_t (*shell_commnad)();
};
typedef struct shell_function shell_function_t;

typedef size_t status;
typedef status (*shell_command)(char* command);

void init_shell(shell_function_t functions[]);