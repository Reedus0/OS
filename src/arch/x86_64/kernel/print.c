#include "include/print.h"
#include "arch/x86_64/include/print.h"

static void clear_row(size_t row) {
    struct Char empty;
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
    update_cursor(g_current_column, g_current_row);
}

void print_newline() {
    g_current_column = 0;

    if (g_current_row < ROWS - 1) {
        g_current_row++;
        return;
    }

    for (size_t i = 1; i < ROWS; i++) {
        for (size_t j = 0; j < COLUMNS; j++) {
            struct Char current_char = *(g_buffer + j + (i * COLUMNS));
            *(g_buffer + j + ((i - 1) * COLUMNS)) = current_char;
        }
    }

    clear_row(ROWS - 1);
}

void print_char(char character) {
    if (character == '\n') {
        print_newline();
        update_cursor(g_current_column, g_current_row);
        return;
    }

    if (character == '\b') {
        delete_char();
        return;
    }

    if (g_current_column > COLUMNS - 1) {
        print_newline();
    }

    struct Char new_char;

    new_char.color = WHITE_COLOR;
    new_char.character = character;

    g_buffer[g_current_column + COLUMNS * g_current_row] = new_char;

    g_current_column++;
    update_cursor(g_current_column, g_current_row);
}

void delete_char() {
    struct Char empty;
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
    update_cursor(g_current_column, g_current_row);
}


void print_string(char* string) {
    while(*string != '\0') {
        print_char(*string);
        string++;
    }
}

void print_number(size_t number) {
    int i = 0;
    char* string = 0;

    if (number == 0) {
        print_string("0");
        return;
    }

    while (number > 0) {
        string[i] = number % 10 + 0x30;
        number /= 10;
        i++;
    }

    string[i] = '\0';

    for (int j = 0; j < i / 2; j++) {
        char temp = string[j];
        string[j] = string[i - j - 1];
        string[i - j - 1] = temp;
    }

    print_string(string);
}

void print_hex(size_t number) {
    int i = 0;
    char* string = 0;

    if (number == 0) {
        print_string("0");
        return;
    }

    while (number > 0) {
        size_t current_number = number % 16;
        if(current_number > 9) {
            string[i] = (current_number % 10) + 0x60 + 1;
        } else {
            string[i] = current_number + 0x30;
        }
        number /= 16;
        i++;
    }

    string[i] = '\0';

    for (int j = 0; j < i / 2; j++) {
        char temp = string[j];
        string[j] = string[i - j - 1];
        string[i - j - 1] = temp;
    }

    print_string(string);
}