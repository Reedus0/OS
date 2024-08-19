#include "keyboard.h"
#include "include/io.h"

static uint16_t keyboard_port;

static char keyboard_process_key() {
    uint8_t key = in8(keyboard_port);
    return g_key_map[key - 1];
}

static void keyboard_set_port(uint16_t port) {
    keyboard_port = port;
}

static void init_keyboard() {
    keyboard_port = KEYBOARD_DEFAULT_PORT;
}

static void deinit_keyboard() {
    
}

module_t init_keyboard_module() {
    if(g_keyboard_module.deinit != NULL) {
        g_keyboard_module.deinit();
    }
    g_keyboard_module.name = "Keyboard module";

    g_keyboard_module.init = init_keyboard;

    MODULE_FUNCTION(g_keyboard_module, KEYBOARD_PROCESS_KEY) = keyboard_process_key;
    MODULE_FUNCTION(g_keyboard_module, KEYBOARD_SET_PORT) = keyboard_set_port;

    g_keyboard_module.deinit = deinit_keyboard;

    return g_keyboard_module;
}