#pragma once

#include <stdint.h>
#include <stddef.h>

#include "include/module.h"

#define COLUMNS 80
#define ROWS 25

#define WHITE_COLOR 15

struct terminal_char {
    uint8_t character;
    uint8_t color;
};
typedef struct terminal_char terminal_char_t;

#define TTY_PRINT_CHAR 5
#define TTY_DELETE_CHAR 6
#define TTY_CLEAR 7

module_t* init_tty_module();