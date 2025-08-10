#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

#define NONE    ""
#define INFO    "[ INFO ]    "
#define SUCCESS "[ SUCCESS ] "
#define ERROR   "[ ERROR ]   "
#define DEBUG   "[ DEBUG ]   "
#define WARN    "[ WARN ]    " 

int printk(char* level, const char* format, ...);
int kget(char* buffer, size_t count);