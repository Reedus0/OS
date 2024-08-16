#include "stdin.h"

void stdin_add_char(char character) {
    if (g_stdin.size < STDIN_SIZE) {
        g_stdin.buffer[g_stdin.size] = character;
        g_stdin.size += 1;
    }
}

void stdin_delete_char() {
    if (g_stdin.size > 0) {
        g_stdin.buffer[g_stdin.size] = 0;
        g_stdin.size -= 1;
    }
}

char stdin_get_last_char() {
    while (g_stdin.updated == 0);
    g_stdin.updated = 0;
    return g_stdin.buffer[g_stdin.size - 1];
}

size_t stdin_get_size() {
    return g_stdin.size;
}

void stdin_update() {
    g_stdin.updated = 1;
}

void clear_stdin() {
    for (size_t i = 0; i < STDIN_SIZE; i++) {
        g_stdin.buffer[i] = 0;
    }
    g_stdin.size = 0;
}