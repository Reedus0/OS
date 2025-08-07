#include "keyboard.h"
#include "include/io.h"
#include "include/types.h"
#include "asm/io.h"

static byte keyboard_process_key(dev_t* dev) {
    keyboard_data_t* keyboard_data = dev->dev_data;

    uint8_t status = in8(keyboard_data->status);

    if (status & 1 == 0) return NULL;

    uint8_t key = in8(keyboard_data->port);

    if (key == 0x2A | key == 0x36) {
        keyboard_data->caps_mode = 1;
        return NULL;
    }
    if (key == 0xAA | key == 0xB6) {
        keyboard_data->caps_mode = 0;
        return NULL;
    }
    if (keyboard_data->caps_mode) {
        return g_caps_map[key - 1];
    }
    return g_key_map[key - 1];
}

static void init_keyboard(dev_t* dev) {

}

static void deinit_keyboard(dev_t* dev) {

}

module_t* init_keyboard_module() {
    module_t* keyboard_module = kalloc(sizeof(module_t));

    keyboard_module->name = "Keyboard module";

    keyboard_module->init = init_keyboard;

    MODULE_FUNCTION(keyboard_module, SDEV_DRIVER_READ_BYTE) = keyboard_process_key;

    keyboard_module->deinit = deinit_keyboard;

    return keyboard_module;
}

dev_t* init_keyboard_dev(uint16_t port, uint16_t status) {
    dev_t* keyboard_dev = kalloc(sizeof(dev_t));

    keyboard_data_t* keyboard_data = kalloc(sizeof(keyboard_data_t));

    keyboard_data->port = port;
    keyboard_data->status = status;

    keyboard_dev->dev_data = keyboard_data;
    keyboard_dev->driver = init_keyboard_module(keyboard_dev);
    keyboard_dev->driver->init(keyboard_dev);

    return keyboard_dev;
}

void deinit_keyboard_dev(dev_t* keyboard_dev) {
    keyboard_dev->driver->deinit(keyboard_dev);

    kfree(keyboard_dev->dev_data);
    kfree(keyboard_dev);
}