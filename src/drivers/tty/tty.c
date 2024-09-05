#include "tty.h"

static size_t current_column;
static size_t current_row;
static terminal_char_t* tty_buffer;

static void tty_enable_cursor(uint8_t cursor_start, uint8_t cursor_end) {
	out8(0x3D4, 0x0A);
	out8(0x3D5, (in8(0x3D5) & 0xC0) | cursor_start);

	out8(0x3D4, 0x0B);
	out8(0x3D5, (in8(0x3D5) & 0xE0) | cursor_end);
}

static void tty_disable_cursor() {
	out8(0x3D4, 0x0A);
	out8(0x3D5, 0x20);
}

static void tty_update_cursor(uint8_t x, uint8_t y, uint8_t max_columns) {
	uint16_t pos = y * max_columns + x;

	out8(0x3D4, 0x0F);
	out8(0x3D5, (uint8_t)(pos & 0xFF));
	out8(0x3D4, 0x0E);
	out8(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

static void tty_clear_row(size_t row) {
    terminal_char_t empty;
    empty.color = WHITE_COLOR;
    empty.character = ' ';

    for (size_t i = 0; i < COLUMNS; i++) {
        *(tty_buffer + i + (row * COLUMNS)) = empty;
    }
}

static void tty_print_clear() {
    for (size_t i = 0; i < ROWS; i++) {
        tty_clear_row(i);
    }

    current_column = 0;
    current_row = 0;
    tty_update_cursor(current_column, current_row, COLUMNS);
}

static void tty_print_newline() {
    current_column = 0;

    if (current_row < ROWS - 1) {
        current_row++;
        return;
    }

    for (size_t i = 1; i < ROWS; i++) {
        for (size_t j = 0; j < COLUMNS; j++) {
            terminal_char_t current_char = *(tty_buffer + j + (i * COLUMNS));
            *(tty_buffer + j + ((i - 1) * COLUMNS)) = current_char;
        }
    }

    tty_clear_row(ROWS - 1);
}

static void tty_delete_char() {
    terminal_char_t empty;
    if (current_column == 0) {
        if (current_row > 0) {
            current_row -= 1;
            current_column = 80;
        } else {
            current_column = 1;
        }
    }

    current_column -= 1;

    empty.color = WHITE_COLOR;
    empty.character = ' ';
    tty_buffer[current_column + COLUMNS * current_row] = empty;
    tty_update_cursor(current_column, current_row, COLUMNS);
}

static void tty_print_char(char character) {
    if (character == '\n') {
        tty_print_newline();
        tty_update_cursor(current_column, current_row, COLUMNS);
        return;
    }

    if (character == '\b') {
        tty_delete_char();
        return;
    }

    if (current_column > COLUMNS - 1) {
        tty_print_newline();
    }

    terminal_char_t new_char;

    new_char.color = WHITE_COLOR;
    new_char.character = character;

    tty_buffer[current_column + COLUMNS * current_row] = new_char;

    current_column++;
    tty_update_cursor(current_column, current_row, COLUMNS);
}

static void init_tty() {
    current_column = 0;
    current_row = 0;
    tty_buffer = (terminal_char_t*)0xb8000;
}

static void deinit_tty() {
    
}

module_t* init_tty_module() {
    module_t* tty_module = kalloc(sizeof(module_t));

    tty_module->name = "TTY module";

    tty_module->init = init_tty;

    MODULE_FUNCTION(tty_module, TTY_PRINT_CHAR) = tty_print_char;
    MODULE_FUNCTION(tty_module, TTY_DELETE_CHAR) = tty_delete_char;
    MODULE_FUNCTION(tty_module, TTY_CLEAR) = tty_print_clear;

    tty_module->deinit = deinit_tty;

    return tty_module;
}