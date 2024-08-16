#include "stdin.h"

void stdin_add_byte(byte new_byte) {
    if (g_stdin.size < STDIN_BUFFER_SIZE) {
        stream_add_byte(&g_stdin, new_byte);
    }
}

void stdin_delete_byte() {
    if (g_stdin.size > 0) {
        stream_delete_byte(&g_stdin);
    }
}

byte stdin_get_last_byte() {
    return stream_get_last_byte(&g_stdin);
}

size_t stdin_get_size() {
    return stream_get_size(&g_stdin);
}

void stdin_update() {
    stream_update(&g_stdin);
}

void clear_stdin() {
    clear_stream(&g_stdin);
}