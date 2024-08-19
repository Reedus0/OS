#pragma once

#include <stdint.h>
#include <stddef.h>

#define MODULE_FUNCTION(module_instance, function) (module_instance.functions[function])

struct module {
    char* name;
    void (*init)();
    size_t (*functions[16])();
    void (*deinit)();
};
typedef struct module module_t;

module_t g_modules[16];
size_t g_modules_count;

void register_module(module_t module) {
    g_modules[g_modules_count] = module;
    g_modules_count += 1;

    module.init();
}