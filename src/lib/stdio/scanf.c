#include "include/io.h"
#include "scanf.h"

int scanf(const char* format, ...) {
    int result = 0;
    size_t current_arg = 0;

    va_list arg;
    va_start(arg, format);
    
    clear_io_buffer();
 
    while(*format != '\0') {
        char current_char = *format;
        if(current_char == '%') {
            char type = *(format + 1);
            switch(type) {
                case 's':
                    current_arg = va_arg(arg, char*);
                    int index = 0;
                    while (1) {
                        char last_char = io_buffer_get_last_char();
                        if (last_char != '\b') {
                            print_char(last_char);
                        }
                        if (last_char == '\n') {
                            break;
                        }
                        if (last_char == '\b') {
                            *((char*)current_arg + index) = 0;
                            if (index > 0) {
                                print_char(last_char);
                                index--;
                            }
                            continue;
                        }
                        *((char*)current_arg + index) = last_char;
                        index++;
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