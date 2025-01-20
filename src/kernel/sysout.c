#include "sysout.h"
#include "include/dev.h"
#include "drivers/tty/tty.h"

static void sysout_print(byte character) {
    if (g_terminal.driver == NULL) return;
    if (character == '\b') {
        MODULE_FUNCTION(g_terminal.driver, TTY_DELETE_CHAR)();
        return;
    }
    MODULE_FUNCTION(g_terminal.driver, TTY_PRINT_CHAR)(character);
}

void sysout_add_string(byte* string) {
    while (*string != '\0') {
        sysout_print(*string);
        string++;
    }
}

void sysout_add_byte(byte new_byte) {
    sysout_print(new_byte);
}