#include "utils.h"
#include "include/print.h"
#include "include/module.h"
#include "include/list.h"
#include "include/macro.h"

shell_command sh_hello(char* command) {
    printk("Hello, Kernel!\n");
    return 0;
} 

shell_command sh_clear(char* command) {
    print_clear();
    return 0;
}

shell_command sh_module(char* command) {
    module_t* current_module = g_modules;
    while (1) {
        printk("%s\n", current_module->name);
        if (current_module->list.prev == NULL) {
            break;
        }
        current_module = container_of(current_module->list.prev, module_t, list);
    }
    return 0;
}