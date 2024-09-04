#pragma once

#include <stdint.h>
#include <stddef.h>

#include "include/types.h"
#include "include/stream.h"

#define STDOUT_BUFFER_SIZE 256

bool g_stdout_updated;
byte g_stdout_buffer[STDOUT_BUFFER_SIZE];

stream_t g_stdout = {
    .buffer = g_stdout_buffer,
    .ended = 0,
    .size = 0
};

void stdout_add_byte(byte character);