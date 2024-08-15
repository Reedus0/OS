#include "driver.h"

void register_driver(driver_t driver) {
    g_drivers[g_drivers_count] = driver;
    g_drivers_count += 1;
}

void init_drivers() {
    for (size_t i; i < g_drivers_count; i++) {
        driver_t* current_driver = &g_drivers[i];
        if(current_driver->init != NULL) {
            current_driver->init();
        }
    }
}