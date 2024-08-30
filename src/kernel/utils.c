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
    char* arg = strchr(command, ' ') + 1;
    if (strlen(arg) == 0) {
        printk("Specify directory!\n");
        return 1;
    }
    if (strcmp(arg, ".")) {
        return 0;
    }
    if (strcmp(arg, "..")) {
        g_shell_dir = g_shell_dir->parent;
        return 0;
    }

    list_t* last_list = &g_shell_dir->subdirs;
    while (last_list->next != NULL) {
        last_list = last_list->next;
        vfs_dir_t* current_dir = container_of(last_list, vfs_dir_t, list);
        if (strcmp(arg, current_dir->name)) {
            g_shell_dir = current_dir;
            return 0;
        }
    }
    printk("Unknown directory!\n");
    return 1;
}

shell_command sh_ls(char* command) {
    list_t* last_list = &g_shell_dir->subdirs;
    while (last_list->next != NULL) {
        last_list = last_list->next;
        vfs_dir_t* current_dir = container_of(last_list, vfs_dir_t, list);
        printk("%s\n", current_dir->name);
    }

    last_list = &g_shell_dir->files;
    while (last_list->next != NULL) {
        last_list = last_list->next;
        vfs_file_t* current_file = container_of(last_list, vfs_file_t, list);
        printk("%s\n", current_file->name);
    }
    return 0;
}

shell_command sh_fc(char* command) {
    return 0;
}