#include "driver.h"

#include "arch/x86_64/drivers/pic/pic.h"

void register_driver(struct driver driver) {
    g_drivers[g_drivers_count] = driver;
    g_drivers_count += 1;
}

void init_drivers() {
    for (size_t i; i < g_drivers_count; i++) {
        struct driver* current_driver = &g_drivers[i];
        if(current_driver->init != NULL) {
            current_driver->init();
        }
    }
}