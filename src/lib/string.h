#pragma once

#include <stdint.h>
#include <stddef.h>

#include "include/types.h"
#include "include/macro.h"

int strlen(const char* string);
bool strncmp(const char* first, const char* second, int n);
bool strcmp(const char* first, const char* second);
char* strchr(const char* string, char character);