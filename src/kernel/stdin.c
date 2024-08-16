#include "stdin.h"

void stdin_add_char(char character) {
    if (g_stdin.size < STDIN_BUFFER_SIZE) {
        stream_add_byte(&g_stdin, character);
    }
}

void stdin_delete_char() {
    if (g_stdin.size > 0) {
        stream_delete_byte(&g_stdin);
    }
}

char stdin_get_last_char() {
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