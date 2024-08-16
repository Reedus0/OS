#pragma once

#include <stdint.h>
#include <stddef.h>

#include "include/types.h"

#define STDIN_SIZE 256

struct stdin {
    char buffer[STDIN_SIZE];
    size_t size;
    byte updated;
} g_stdin;

void stdin_add_char(char character);
void stdin_delete_char();
char stdin_get_last_char();
void stdin_update();
void clear_stdin();
