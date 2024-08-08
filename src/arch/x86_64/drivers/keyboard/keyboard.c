#include "keyboard.h"

static void keyboard_process_key() {
    uint8_t key = inb(KEYBOARD_PORT);
    if(key == 0xE) {
        delete_char();
        return;
    }
    if(g_key_map[key - 1] != NULL) {
        print_char(g_key_map[key - 1]);
    }
}

static void init_keyboard() {

}

static void deinit_keyboard() {

}

struct driver init_keyboard_driver() {
    g_keyboard_driver.init = init_keyboard;

    g_keyboard_driver.functions[KEYBOARD_DRIVER_PROCESS_KEY] = keyboard_process_key;

    g_keyboard_driver.deinit = deinit_keyboard;
}