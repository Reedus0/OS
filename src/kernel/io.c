#include "kernel/sysout.h"
#include "include/task.h"
#include "io.h"
#include "lib/string.h"

int printk(char* level, const char* format, ...) {
    char buffer[128] = { 0 };
    char number[128] = { 0 };
    int padding = 0;
    int result = 0;
    size_t current_arg = 0;

    sysout_add_string(level);

    va_list arg;
    va_start(arg, format);

    while (*format != '\0') {
        char current_char = *format;
        if (current_char == '%') {
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
            switch (*type) {
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
            sysout_add_string(buffer);
            memset(number, 0, 128);
            memset(buffer, 0, 128);
            format += 2;
            result += 2;
            padding = 0;
            continue;
        }
        sysout_add_byte(current_char);
        format++;
        result++;
    }
    va_end(arg);
    return result;
}

int kget(char* buffer, size_t count) {
    task_t* current_task = get_task(g_current_task_id);

    clear_stream(&current_task->stdin);
    size_t last_size = 0;

    while (1) {
        while (!current_task->stdin_updated);
        current_task->stdin_updated = 0;
        byte last_byte = stream_get_last_byte(&current_task->stdin);
        size_t stdin_size = stream_get_size(&current_task->stdin);

        if (stdin_size > last_size) {
            sysout_add_byte(last_byte);
        }
        if (last_byte == '\n') {
            break;
        }
        if (stdin_size <= last_size) {
            if (last_size > 0) {
                sysout_add_byte('\b');
                last_size--;
            }
        }
        if (last_size < count) {
            buffer[last_size] = last_byte;
        }
        last_size = stdin_size;
    }
    return last_size;
}