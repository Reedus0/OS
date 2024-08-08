#pragma once

#include <stdint.h>
#include <stddef.h>

#define driver_function(driver_instance, function) (driver_instance.functions[function])

struct driver {
    void (*init)();
    void (*functions[16])();
    void (*deinit)();
};

struct driver g_drivers[16];
size_t g_drivers_count;

void register_driver(struct driver driver);
void init_drivers();