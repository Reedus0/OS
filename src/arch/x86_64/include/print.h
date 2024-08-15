#pragma once

#define COLUMNS 80
#define ROWS 25

#define WHITE_COLOR 15

struct Char {
    uint8_t character;
    uint8_t color;
};

static struct Char* g_buffer = (struct Char*) 0xb8000;

size_t g_current_column = 0;
size_t g_current_row = 0;