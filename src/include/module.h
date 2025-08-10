#pragma once

#include <stdint.h>
#include <stddef.h>

#include "include/list.h"
#include "include/kalloc.h"
#include "kernel/io.h"

#define MODULE_FUNCTION(module_ptr, function) ((module_ptr)->functions[function])

struct module {
    char* name;
    list_t list;

    void (*init)();
    void (*deinit)();

    size_t(*functions[32])();
};
typedef struct module module_t;