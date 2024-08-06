#include "include/print.h"

#define COLUMNS 80
#define ROWS 25

#define WHITE_COLOR 15

struct Char {
    uint8_t character;
    uint8_t color;
};

struct Char* buffer = (struct Char*) 0xb8000;

size_t current_column = 0;
size_t current_row = 0;

static void clear_row(size_t row) {
    struct Char empty;
    empty.color = WHITE_COLOR;
    empty.character = ' ';

    for (size_t i = 0; i < COLUMNS; i++) {
        *(buffer + i + (row * COLUMNS)) = empty;
    }
}

void print_clear() {
    for (size_t i = 0; i < ROWS; i++) {
        clear_row(i);
    }

    current_column = 0;
    current_row = 0;
}

static void print_newline() {
    current_column = 0;

    if (current_row < ROWS - 1) {
        current_row++;
        return;
    }

    for (size_t i = 1; i < ROWS; i++) {
        for (size_t j = 0; j < COLUMNS; j++) {
            struct Char current_char = *(buffer + i + (j * COLUMNS));
            *(buffer + i + ((j - 1) * COLUMNS)) = current_char;
        }
    }

    clear_row(COLUMNS - 1);
}

void print_char(char character) {
    if(character == '\n') {
        print_newline();
        return;
    }

    if (current_column > COLUMNS) {
        print_newline();
    }

    struct Char new_char;

    new_char.color = WHITE_COLOR;
    new_char.character = character;

    buffer[current_column + COLUMNS * current_row] = new_char;

    current_column++;
}

void print_string(char* string) {
    while(*string != '\0') {
        print_char(*string);
        string++;
    }
}