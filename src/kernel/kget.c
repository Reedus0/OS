#include "kernel/stdin.h"
#include "kget.h"

void kget(byte* buffer) {

    clear_stdin();
    size_t last_size = 0;

    while (1) {
        byte last_byte = stdin_get_last_byte();
        size_t stdin_size = stdin_get_size();
        if (stdin_size > last_size) {
            print_char(last_byte);
        }
        if (last_byte == '\n') {
            break;
        }
        if (stdin_size <= last_size) {
            if (last_size > 0) {
                delete_char();
                last_size--;
            }
        }
        *((char*)buffer + last_size) = last_byte;
        last_size = stdin_size;
    };
}