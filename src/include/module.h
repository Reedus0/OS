#pragma once

#include <stdint.h>
#include <stddef.h>

#include "include/list.h"

#define MODULE_FUNCTION(module_instance, function) (module_instance.functions[function])

struct module {
    char* name;
    list_t list;
    void (*init)();
    size_t (*functions[16])();
    void (*deinit)();
};
typedef struct module module_t;

module_t* g_modules;

void register_module(module_t* module) {
    if (g_modules != NULL) {
        list_insert(&g_modules->list, &module->list);
    }
    g_modules = module;

    module->init();
}