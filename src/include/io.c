#include "io.h"

void io_buffer_add_char(char character) {
    if (g_io_buffer.size < IO_BUFFER_SIZE) {
        g_io_buffer.buffer[g_io_buffer.size] = character;
        g_io_buffer.size += 1;
    }
}

void io_buffer_delete_char() {
    if (g_io_buffer.size > -1) {
        g_io_buffer.buffer[g_io_buffer.size] = 0;
        g_io_buffer.size -= 1;
    }
}

char io_buffer_get_last_char() {
    while (g_io_buffer.updated == 0);
    g_io_buffer.updated = 0;
    return g_io_buffer.buffer[g_io_buffer.size - 1];
}

void io_buffer_update() {
    g_io_buffer.updated = 1;
}

void clear_io_buffer() {
    for (size_t i = 0; i < IO_BUFFER_SIZE; i++) {
        g_io_buffer.buffer[i] = 0;
    }
    g_io_buffer.size = 0;
}