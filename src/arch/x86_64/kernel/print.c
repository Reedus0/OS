#include "include/print.h"
#include "include/cursor.h"

#define COLUMNS 80
#define ROWS 25

#define WHITE_COLOR 15

struct terminal_char {
    uint8_t character;
    uint8_t color;
};
typedef struct terminal_char terminal_char_t;

static terminal_char_t* g_buffer = (terminal_char_t*) 0xb8000;

static size_t g_current_column = 0;
static size_t g_current_row = 0;

static void clear_row(size_t row) {
    terminal_char_t empty;
    empty.color = WHITE_COLOR;
    empty.character = ' ';

    for (size_t i = 0; i < COLUMNS; i++) {
        *(g_buffer + i + (row * COLUMNS)) = empty;
    }
}

void print_clear() {
    for (size_t i = 0; i < ROWS; i++) {
        clear_row(i);
    }

    g_current_column = 0;
    g_current_row = 0;
    update_cursor(g_current_column, g_current_row, COLUMNS);
}

void print_newline() {
    g_current_column = 0;

    if (g_current_row < ROWS - 1) {
        g_current_row++;
        return;
    }

    for (size_t i = 1; i < ROWS; i++) {
        for (size_t j = 0; j < COLUMNS; j++) {
            terminal_char_t current_char = *(g_buffer + j + (i * COLUMNS));
            *(g_buffer + j + ((i - 1) * COLUMNS)) = current_char;
        }
    }

    clear_row(ROWS - 1);
}

void print_char(char character) {
    if (character == '\n') {
        print_newline();
        update_cursor(g_current_column, g_current_row, COLUMNS);
        return;
    }

    if (character == '\b') {
        delete_char();
        return;
    }

    if (g_current_column > COLUMNS - 1) {
        print_newline();
    }

    terminal_char_t new_char;

    new_char.color = WHITE_COLOR;
    new_char.character = character;

    g_buffer[g_current_column + COLUMNS * g_current_row] = new_char;

    g_current_column++;
    update_cursor(g_current_column, g_current_row, COLUMNS);
}

void delete_char() {
    terminal_char_t empty;
    if (g_current_column == 0) {
        if (g_current_row > 0) {
            g_current_row -= 1;
            g_current_column = 80;
        } else {
            g_current_column = 1;
        }
    }

    g_current_column -= 1;

    empty.color = WHITE_COLOR;
    empty.character = ' ';
    g_buffer[g_current_column + COLUMNS * g_current_row] = empty;
    update_cursor(g_current_column, g_current_row, COLUMNS);
}


void print_string(char* string) {
    while(*string != '\0') {
        print_char(*string);
        string++;
    }
}

static char* itoa(size_t num, char* str, int radix) {
    size_t i = 0;

    if (num == 0) {
        *str = '0';
        *(str + 1) = '\0';
        return str;
    }

    while (num > 0) {
        size_t current_number = num % radix;
        if(current_number > 9) {
            *(str + i) = (current_number % 10) + 0x60 + 1;
        } else {
            *(str + i) = current_number + 0x30;
        }
        num /= radix;
        i++;
    }

    *(str + i) = '\0';

    for (int j = 0; j < i / 2; j++) {
        char tmp = *(str + j);
        *(str + j) = *(str + i - j - 1);
        *(str + i - j - 1) = tmp;
    }

    return str;
}

void print_number(size_t number) {
    char buffer[256];

    itoa(number, buffer, 10);

    print_string(buffer);
}

void print_hex(size_t number) {
    char buffer[256];

    itoa(number, buffer, 16);

    print_string(buffer);
}