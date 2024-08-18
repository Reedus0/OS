#include "include/print.h"
#include "printk.h"

int printk(const char* format, ...) {
    int result = 0;
    size_t current_arg = 0;

    va_list arg;
    va_start(arg, format);

    while(*format != '\0') {
        char current_char = *format;
        if(current_char == '%') {
            char type = *(format + 1);
            switch(type) {
                case 's':
                    current_arg = va_arg(arg, char*);
                    print_string(current_arg);
                    break;
                case 'd':
                    current_arg = va_arg(arg, size_t);
                    print_number(current_arg);
                    break;
                case 'x':
                case 'p':
                    current_arg = va_arg(arg, size_t);
                    print_hex(current_arg);
                    break;
                case 'c':
                default:
                    current_arg = va_arg(arg, char);
                    print_char(current_arg);
                    break;
            }
            format += 2;
            result += 2;
            continue;
        }
        print_char(current_char);
        format++;
        result++;
    }
    va_end(arg);
    return result;
}