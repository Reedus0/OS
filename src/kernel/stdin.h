#pragma once

#include <stdint.h>
#include <stddef.h>

#include "include/types.h"
#include "impl/stream.h"

#define STDIN_BUFFER_SIZE 256

byte g_stdin_buffer[STDIN_BUFFER_SIZE];

stream_t g_stdin = {
    g_stdin_buffer,
    0,
    0
};

void stdin_add_byte(byte character);
void stdin_delete_byte();
byte stdin_get_last_byte();
void stdin_update();
void clear_stdin();