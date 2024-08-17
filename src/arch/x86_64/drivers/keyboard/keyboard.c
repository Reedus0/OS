#include "keyboard.h"
#include "include/print.h"
#include "arch/x86_64/include/print.h"
#include "arch/x86_64/asm/io.h"

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end) {
	out8(0x3D4, 0x0A);
	out8(0x3D5, (in8(0x3D5) & 0xC0) | cursor_start);

	out8(0x3D4, 0x0B);
	out8(0x3D5, (in8(0x3D5) & 0xE0) | cursor_end);
}

void update_cursor(int x, int y) {
	uint16_t pos = y * COLUMNS + x;

	out8(0x3D4, 0x0F);
	out8(0x3D5, (uint8_t)(pos & 0xFF));
	out8(0x3D4, 0x0E);
	out8(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void disable_cursor() {
	out8(0x3D4, 0x0A);
	out8(0x3D5, 0x20);
}

static char keyboard_process_key() {
    uint8_t key = in8(KEYBOARD_PORT);
    return g_key_map[key - 1];
}

static void init_keyboard() {
    enable_cursor(14, 15);
}

static void deinit_keyboard() {
    disable_cursor();
}

driver_t init_keyboard_driver() {
    if(g_keyboard_driver.deinit != NULL) {
        g_keyboard_driver.deinit();
    }
    g_keyboard_driver.name = "Keyboard driver";

    g_keyboard_driver.init = init_keyboard;

    driver_function(g_keyboard_driver, KEYBOARD_DRIVER_PROCESS_KEY) = keyboard_process_key;

    g_keyboard_driver.deinit = deinit_keyboard;

    return g_keyboard_driver;
}