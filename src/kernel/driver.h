#pragma once

#include <stdint.h>
#include <stddef.h>

#define driver_function(driver_instance, function) (driver_instance.functions[function])

struct driver {
    void (*init)();
    size_t (*functions[16])();
    void (*deinit)();
};
typedef struct driver driver_t;

driver_t g_drivers[16];
size_t g_drivers_count;

void register_driver(driver_t driver);
void init_drivers();