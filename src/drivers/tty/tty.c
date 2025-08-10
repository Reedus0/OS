#include "tty.h"

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

static void tty_clear_row(terminal_char_t* buffer, size_t row) {
    terminal_char_t empty;
    empty.color = WHITE_COLOR;
    empty.character = ' ';

    for (size_t i = 0; i < COLUMNS; i++) {
        *(buffer + i + (row * COLUMNS)) = empty;
    }
}

static void tty_print_clear(dev_t* dev) {
    tty_data_t* tty_data = dev->dev_data;

    for (size_t i = 0; i < ROWS; i++) {
        tty_clear_row(tty_data->buffer, i);
    }

    tty_data->current_column = 0;
    tty_data->current_row = 0;
    tty_update_cursor(tty_data->current_column, tty_data->current_row, COLUMNS);
}

static void tty_print_newline(dev_t* dev) {
    tty_data_t* tty_data = dev->dev_data;

    tty_data->current_column = 0;

    if (tty_data->current_row < ROWS - 1) {
        tty_data->current_row++;
        return;
    }

    for (size_t i = 1; i < ROWS; i++) {
        for (size_t j = 0; j < COLUMNS; j++) {
            terminal_char_t current_char = *(tty_data->buffer + j + (i * COLUMNS));
            *(tty_data->buffer + j + ((i - 1) * COLUMNS)) = current_char;
        }
    }

    tty_clear_row(tty_data->buffer, ROWS - 1);
}

static void tty_delete_char(dev_t* dev) {
    tty_data_t* tty_data = dev->dev_data;

    terminal_char_t empty;
    if (tty_data->current_column == 0) {
        if (tty_data->current_row > 0) {
            tty_data->current_row -= 1;
            tty_data->current_column = 80;
        }
        else {
            tty_data->current_column = 1;
        }
    }

    tty_data->current_column -= 1;

    empty.color = WHITE_COLOR;
    empty.character = ' ';
    tty_data->buffer[tty_data->current_column + COLUMNS * tty_data->current_row] = empty;
    tty_update_cursor(tty_data->current_column, tty_data->current_row, COLUMNS);
}

static void tty_print_char(dev_t* dev, char character) {
    tty_data_t* tty_data = dev->dev_data;

    if (character == '\n') {
        tty_print_newline(dev);
        tty_update_cursor(tty_data->current_column, tty_data->current_row, COLUMNS);
        return;
    }

    if (character == '\b') {
        tty_delete_char(dev);
        return;
    }

    if (tty_data->current_column > COLUMNS - 1) {
        tty_print_newline(dev);
    }

    terminal_char_t new_char;

    new_char.color = WHITE_COLOR;
    new_char.character = character;

    tty_data->buffer[tty_data->current_column + COLUMNS * tty_data->current_row] = new_char;

    tty_data->current_column++;
    tty_update_cursor(tty_data->current_column, tty_data->current_row, COLUMNS);
}

static void init_tty(dev_t* dev) {
    tty_print_clear(dev);
}

static void deinit_tty(dev_t* dev) {

}

module_t* init_tty_module() {
    module_t* tty_module = kalloc(sizeof(module_t));

    tty_module->name = "TTY module";

    tty_module->init = init_tty;

    MODULE_FUNCTION(tty_module, SDEV_DRIVER_WRITE_BYTE) = tty_print_char;
    MODULE_FUNCTION(tty_module, TTY_CLEAR) = tty_print_clear;

    tty_module->deinit = deinit_tty;

    return tty_module;
}

dev_t* init_tty_dev() {
    dev_t* tty_dev = kalloc(sizeof(dev_t));

    tty_data_t* tty_data = kalloc(sizeof(tty_data_t));

    tty_data->current_column = 0;
    tty_data->current_row = 0;
    tty_data->buffer = (terminal_char_t*)0xb8000;

    tty_dev->dev_data = tty_data;
    tty_dev->driver = init_tty_module(tty_dev);
    tty_dev->driver->init(tty_dev);

    return tty_dev;
}

void deinit_tty_dev(dev_t* tty_dev) {
    tty_dev->driver->deinit(tty_dev);

    kfree(tty_dev->dev_data);
    kfree(tty_dev);
}