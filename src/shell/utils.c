#include "utils.h"
#include "include/module.h"
#include "include/list.h"
#include "include/macro.h"
#include "drivers/tty/tty.h"
#include "kernel/printk.h"
#include "kernel/kget.h"
#include "lib/string.h"

shell_command sh_hello(char* command) {
    printk(NONE, "Hello, Kernel!\n");
    return 0;
} 

shell_command sh_clear(char* command) {
    MODULE_FUNCTION(g_terminal.driver, TTY_CLEAR)();
    return 0;
}

shell_command sh_module(char* command) {
    module_t* current_module = g_modules;
    while (1) {
        printk(NONE, "%s\n", current_module->name);
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
        printk(NONE, "Specify directory!\n");
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
    printk(NONE, "Unknown directory!\n");
    return 1;
}

shell_command sh_ls(char* command) {
    list_t* last_list = &g_shell_dir->subdirs;
    while (last_list->next != NULL) {
        last_list = last_list->next;
        vfs_dir_t* current_dir = container_of(last_list, vfs_dir_t, list);
        printk(NONE, "%s\n", current_dir->name);
    }

    last_list = &g_shell_dir->files;
    while (last_list->next != NULL) {
        last_list = last_list->next;
        vfs_file_t* current_file = container_of(last_list, vfs_file_t, list);
        printk(NONE, "%s\n", current_file->name);
    }
    return 0;
}

shell_command sh_mkfile(char* command) {
    char* arg = strchr(command, ' ') + 1;
    
    vfs_create_file(arg);
    return 0;
}

shell_command sh_mkdir(char* command) {
    char* arg = strchr(command, ' ') + 1;
    
    vfs_create_dir(arg);
    return 0;
}

shell_command sh_rm(char* command) {
    char* arg = strchr(command, ' ') + 1;
    
    vfs_delete_file(arg);
    return 0;
}

shell_command sh_rmdir(char* command) {
    char* arg = strchr(command, ' ') + 1;
    
    vfs_delete_dir(arg);
    return 0;
}

shell_command sh_read(char* command) {
    byte buffer[256] = {0};
    char* arg = strchr(command, ' ') + 1;
    
    file_t* file = vfs_open_file(arg);
    size_t bytes_read = vfs_read_file(file, buffer, 256);

    for (size_t i = 0; i < bytes_read; i++) {
        printk(NONE, "%x", buffer[i]);
    }

    vfs_close_file(file);
    printk(NONE, "\n");

    return 0;
}

shell_command sh_write(char* command) {
    byte buffer[256] = {0};
    char* arg = strchr(command, ' ') + 1;
    
    file_t* file = vfs_open_file(arg);

    kget(buffer);

    vfs_write_file(file, buffer, 256);
    vfs_close_file(file);
    printk(NONE, "\n");

    return 0;
}