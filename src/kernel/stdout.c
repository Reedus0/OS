#include "stdout.h"

void stdout_add_byte(byte new_byte) {
    if (g_stdout.size < STDOUT_BUFFER_SIZE) {
        stream_add_byte(&g_stdout, new_byte);
    }
}

