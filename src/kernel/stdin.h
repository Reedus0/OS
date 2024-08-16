#pragma once

#include <stdint.h>
#include <stddef.h>

#include "include/types.h"
#include "kernel/stream.h"

#define STDIN_BUFFER_SIZE 256

char g_stdin_buffer[STDIN_BUFFER_SIZE];

stream_t g_stdin = {
    g_stdin_buffer,
    0,
    0
};

void stdin_add_char(char character);
void stdin_delete_char();
char stdin_get_last_char();
void stdin_update();
void clear_stdin();