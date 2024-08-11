#include "keyboard.h"
#include "include/print.h"
#include "arch/x86_64/asm/io.h"

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

driver_t init_keyboard_driver() {
    if(g_keyboard_driver.deinit != NULL) {
        g_keyboard_driver.deinit();
    }
    g_keyboard_driver.init = init_keyboard;

    driver_function(g_keyboard_driver, KEYBOARD_DRIVER_PROCESS_KEY) = keyboard_process_key;

    g_keyboard_driver.deinit = deinit_keyboard;
}