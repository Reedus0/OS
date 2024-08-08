#pragma once

#include <stdint.h>
#include <stddef.h>

void print_clear();
void print_newline();
void print_char(char character);
void print_string(char* string);
void print_number(size_t number);
void print_hex(size_t number);