#include "utils.h"
#include "include/print.h"
#include "kernel/driver.h"

shell_command sh_hello(char* command) {
    printf("Hello, Kernel!\n");
    return 0;
}

shell_command sh_clear(char* command) {
    print_clear();
    return 0;
}

shell_command sh_driver(char* command) {
    for (size_t i = 0; i < g_drivers_count; i++) {
        if (i > 16) break;
        driver_t* current_driver = &g_drivers[i];
        printf("%s\n", current_driver->name);
    }
    return 0;
}