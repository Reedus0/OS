#include "utils.h"
#include "include/print.h"
#include "include/module.h"

shell_command sh_hello(char* command) {
    printk("Hello, Kernel!\n");
    return 0;
}

shell_command sh_clear(char* command) {
    print_clear();
    return 0;
}

shell_command sh_module(char* command) {
    for (size_t i = 0; i < g_modules_count; i++) {
        if (i > 16) break;
        module_t* current_module = &g_modules[i];
        printk("%s\n", current_module->name);
    }
    return 0;
}