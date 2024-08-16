#include "driver.h"
#include "include/print.h"
#include "kernel/driver.h"

shell_command sh_driver(char* command) {
    for (size_t i = 0; i < g_drivers_count; i++) {
        driver_t* current_driver = &g_drivers[i];
        printf("%s:\n    %s\n", current_driver->name, current_driver->info());
    }
    return 0;
}