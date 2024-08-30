#pragma once

#include <stdint.h>
#include <stddef.h>

#include "include/types.h"
#include "include/macro.h"

int strlen(const char* string);
bool strncmp(const char* first, const char* second, int n);
bool strcmp(const char* first, const char* second);
char* strchr(const char* string, char character);
char* strrchr(const char* string, char character);
int strncpy(char* dest, const char* src, int n);

char tolower(char character);
char toupper(char character);

char* ltrim(char* string);
char* rtrim(char* string);
char* trim_string(char* string);

void* memset(byte* dest, byte c, int count);
void* memcpy(byte* dest, byte* src, int count);