#include "utils.h"
#include "include/print.h"
#include "include/module.h"
#include "include/list.h"
#include "include/macro.h"
#include "lib/string.h"

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

shell_command sh_cd(char* command) {
    char* arg = strchr(command, ' ');
    if (strlen(arg) == 0) {
        printk("Specify folder!\n");
        return 1;
    }
    if (strcmp(arg, ".")) {
        return 0;
    }
    if (strcmp(arg, "..")) {
        g_shell_path = g_shell_path->parent;
        return 0;
    }

    list_t* last_list = &g_shell_path->subdirs;
    while (last_list->next != NULL) {
        last_list = last_list->next;
        dir_t* current_dir = container_of(last_list, dir_t, subdirs);
        if (strcmp(arg, current_dir->name)) {
            g_shell_path = current_dir;
        }
    }
    return 0;
}

shell_command sh_ls(char* command) {
    list_t* last_list = &g_shell_path->subdirs;
    while (last_list->next != NULL) {
        last_list = last_list->next;
        dir_t* current_dir = container_of(last_list, dir_t, subdirs);
        printk("%s\n", current_dir->name);
    }
    return 0;
}