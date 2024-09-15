#include "kernel/stdin.h"
#include "kernel/stdout.h"
#include "kget.h"

void kget(byte* buffer) {

    clear_stdin();
    size_t last_size = 0;

    while (1) {
        byte last_byte = stdin_get_last_byte();
        size_t stdin_size = stdin_get_size();
        if (stdin_size > last_size) {
            stdout_add_byte(last_byte);
        }
        if (last_byte == '\n') {
            break;
        }
        if (stdin_size <= last_size) {
            if (last_size > 0) {
                stdout_add_byte('\b');
                last_size--;
            }
        }
        buffer[last_size] = last_byte;
        last_size = stdin_size;
    }
}