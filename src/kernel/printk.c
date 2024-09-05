#include "kernel/stdout.h"
#include "printk.h"

static char* itoa(size_t num, char* str, int radix) {
    size_t i = 0;

    if (num == 0) {
        *str = '0';
        *(str + 1) = '\0';
        return str;
    }

    while (num > 0) {
        size_t current_number = num % radix;
        if(current_number > 9) {
            *(str + i) = (current_number % 10) + 0x60 + 1;
        } else {
            *(str + i) = current_number + 0x30;
        }
        num /= radix;
        i++;
    }

    *(str + i) = '\0';

    for (int j = 0; j < i / 2; j++) {
        char tmp = *(str + j);
        *(str + j) = *(str + i - j - 1);
        *(str + i - j - 1) = tmp;
    }

    return str;
}

int printk(const char* format, ...) {
    char buffer[256];
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
                    stdout_add_string(current_arg);
                    break;
                case 'd':
                    current_arg = va_arg(arg, size_t);
                    itoa(current_arg, buffer, 10);
                    stdout_add_string(buffer);
                    break;
                case 'x':
                case 'p':
                    current_arg = va_arg(arg, size_t);
                    itoa(current_arg, buffer, 16);
                    stdout_add_string(buffer);
                    break;
                case 'c':
                default:
                    current_arg = va_arg(arg, size_t);
                    itoa(current_arg, buffer, 10);
                    stdout_add_string(buffer);
                    break;
            }
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