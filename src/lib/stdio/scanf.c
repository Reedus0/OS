#include "kernel/stdin.h"
#include "scanf.h"

int scanf(const char *format, ...) {
    int result = 0;
    size_t current_arg = 0;

    va_list arg;
    va_start(arg, format);
    
    clear_stdin();

    while(*format != '\0') {
        char current_char = *format;
        if(current_char == '%') {
            char type = *(format + 1);
            switch(type) {
                case 's':
                    current_arg = va_arg(arg, char*);
                    size_t last_size = 0;
                    size_t read = 1;
                    while (1) {
                        byte last_byte = stdin_get_last_byte();
                        size_t stdin_size = stdin_get_size();
                        if (stdin_size > last_size) {
                            print_char(last_byte);
                        }
                        if (last_byte == ' ') {
                            read = 0;
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
                        if (read) {
                            *((char*)current_arg + last_size) = last_byte;
                            last_size = stdin_size;
                        }
                    };
                    break;
                default:
                    break;
            }
            format += 2;
            result += 1;
            continue;
        }
        format++;
    }
    va_end(arg);
    return result;
}