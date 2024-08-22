#pragma once

#include <stdint.h>
#include <stddef.h>

#include "include/list.h"
#include "include/kalloc.h"

#define MODULE_FUNCTION(module_ptr, function) ((module_ptr)->functions[function])

struct module {
    char* name;
    list_t list;

    void (*init)();
    void (*deinit)();
    
    size_t (*functions[32])();
};
typedef struct module module_t;

module_t* g_modules;

void register_module(module_t* module) {
    if (g_modules != NULL) {
        list_insert_after(&g_modules->list, &module->list);
    }
    g_modules = module;

    module->init();
}

void unregister_module(module_t* module) {
    if (g_modules != NULL) {
        list_remove(&module->list);
    }

    module->deinit();
    kfree(module);
}