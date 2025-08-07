#pragma once

#include <stdint.h>
#include <stddef.h>

#include "include/module.h"
#include "include/dev.h"

#define COLUMNS 80
#define ROWS 25

#define WHITE_COLOR 15

struct terminal_char {
    uint8_t character;
    uint8_t color;
};
typedef struct terminal_char terminal_char_t;

struct tty_data {
    size_t current_column;
    size_t current_row;
    terminal_char_t* buffer;
};
typedef struct tty_data tty_data_t;


#define TTY_PRINT_CHAR 5
#define TTY_DELETE_CHAR 6
#define TTY_CLEAR 7

module_t* init_tty_module();
dev_t* init_tty_dev();
void deinit_tty_dev(dev_t* tty_dev);