#include "keyboard.h"

void keyboard_process_key() {
    uint8_t key = inb(KEYBOARD_PORT);
    if(key == 0xE) {
        delete_char();
        return;
    }
    if(key_map[key - 1] != NULL) {
        print_char(key_map[key - 1]);
    }
}