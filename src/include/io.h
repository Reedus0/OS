#pragma once

#include <stdint.h>
#include <stddef.h>

#define IO_BUFFER_SIZE 256

struct io_buffer {
    char buffer[IO_BUFFER_SIZE];
    size_t size;
    char updated;
} g_io_buffer;

void io_buffer_add_char(char character);
void io_buffer_delete_char();
char io_buffer_get_last_char();
void io_buffer_update();
void clear_io_buffer();
