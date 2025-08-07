#include "sysout.h"
#include "include/dev.h"
#include "drivers/tty/tty.h"

static void sysout_print(byte character) {
    sdev_write(g_terminal, character);
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