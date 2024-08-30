#include "keyboard.h"
#include "include/io.h"
#include "include/dev.h"
#include "include/types.h"

static uint16_t keyboard_port;
static bool caps_mode;

static void keyboard_enable_caps() {
    caps_mode = 1;
}

static void keyboard_disable_caps() {
    caps_mode = 0;
}

static byte keyboard_process_key() {
    uint8_t key = in8(keyboard_port);
    if (key == 0x2A | key == 0x36) {
        keyboard_enable_caps();
        return 0;
    }
    if (key == 0xAA | key == 0xB6) {
        keyboard_disable_caps();
        return 0;
    }
    if (caps_mode) {
        return g_caps_map[key - 1];
    }
    return g_key_map[key - 1];
}

static void keyboard_set_port(uint16_t port) {
    keyboard_port = port;
}

static void init_keyboard() {
    keyboard_port = KEYBOARD_DEFAULT_PORT;
    caps_mode = 0;
}

static void deinit_keyboard() {
    
}

module_t* init_keyboard_module() {
    module_t* keyboard_module = kalloc(sizeof(module_t));

    keyboard_module->name = "Keyboard module";

    keyboard_module->init = init_keyboard;

    MODULE_FUNCTION(keyboard_module, SDEV_DRIVER_READ_BYTE) = keyboard_process_key;
    MODULE_FUNCTION(keyboard_module, KEYBOARD_SET_PORT) = keyboard_set_port;

    keyboard_module->deinit = deinit_keyboard;

    return keyboard_module;
}