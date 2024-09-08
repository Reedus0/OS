#include "kernel/stdout.h"
#include "printk.h"
#include "lib/string.h"

int printk(char* level, const char* format, ...) {
    char buffer[64] = { 0 };
    char number[64] = { 0 };
    int padding = 0;
    int result = 0;
    size_t current_arg = 0;

    stdout_add_string(level);

    va_list arg;
    va_start(arg, format);

    while(*format != '\0') {
        char current_char = *format;
        if(current_char == '%') {
            char* type = format + 1;
            if (isdigit(*type)) {
                padding = atoi(type);

                while (isdigit(*type)) {
                    format++;
                    result++;
                    type++;
                }
            }
            int offset = 0;
            switch(*type) {
                case 's':
                    current_arg = va_arg(arg, char*);
                    offset = padding != 0 ? padding - strlen(current_arg) : 0;
                    for (size_t i = 0; i < offset; i++) {
                        buffer[i] = ' ';
                    }
                    strncpy(buffer + offset, current_arg, strlen(current_arg));
                    break;
                case 'd':
                    current_arg = va_arg(arg, size_t);
                    itoa(current_arg, number, 10);
                    offset = padding != 0 ? padding - strlen(number) : 0;
                    for (size_t i = 0; i < offset; i++) {
                        buffer[i] = '0';
                    }
                    strncpy(buffer + offset, number, strlen(number));
                    break;
                case 'x':
                case 'p':
                    current_arg = va_arg(arg, size_t);
                    itoa(current_arg, number, 16);
                    offset = padding != 0 ? padding - strlen(number) : 0;
                    for (size_t i = 0; i < offset; i++) {
                        buffer[i] = '0';
                    }
                    strncpy(buffer + offset, number, strlen(number));
                    break;
                case 'c':
                default:
                    current_arg = va_arg(arg, size_t);
                    itoa(current_arg, number, 10);
                    offset = padding != 0 ? padding - strlen(number) : 0;
                    for (size_t i = 0; i < offset; i++) {
                        buffer[i] = '0';
                    }
                    strncpy(buffer + offset, number, strlen(number));
                    break;
            }
            stdout_add_string(buffer);
            format += 2;
            result += 2;
            continue;
        }
        stdout_add_byte(current_char);
        format++;
        result++;
    }
    va_end(arg);
    return result;
}