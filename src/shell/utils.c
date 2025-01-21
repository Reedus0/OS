#include "utils.h"
#include "include/module.h"
#include "include/list.h"
#include "include/macro.h"
#include "include/time.h"
#include "include/task.h"
#include "drivers/tty/tty.h"
#include "kernel/printk.h"
#include "kernel/syscall.h"
#include "kernel/elf.h"
#include "lib/string.h"

shell_command sh_hello(char* command) {
    __syscall(1, "Hello, Kernel!\n", 15);
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
        printk(NONE, "D | 0000 | %s\n", current_dir->name);
    }

    last_list = &g_shell_dir->files;
    while (last_list->next != NULL) {
        last_list = last_list->next;
        vfs_file_t* current_file = container_of(last_list, vfs_file_t, list);
        printk(NONE, "F | %4x | %s\n", current_file->size, current_file->name);
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
    byte offset[64] = {0};
    char* arg = strchr(command, ' ') + 1;

    printk(NONE, "Offset: ");
    __syscall(0, offset, 64);
    
    file_t* file = vfs_open_file(arg);
    vfs_seek(file, atoi(offset));
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
    byte offset[64] = {0};
    char* arg = strchr(command, ' ') + 1;

    printk(NONE, "Offset: ");
    __syscall(0, offset, 64);
    
    file_t* file = vfs_open_file(arg);
    vfs_seek(file, atoi(offset));
    vfs_read_file(file, buffer, 256);
    
    __syscall(0, offset, 64);

    vfs_write_file(file, buffer, 256);
    vfs_close_file(file);
    printk(NONE, "\n");

    return 0;
}

shell_command sh_time(char* command) {
    struct time ktime = time();
    
    printk(NONE, "%2d:%2d:%2d %2d/%2d/%4d\n", 
        ktime.hour, ktime.minute, ktime.second,
        ktime.day, ktime.month, ktime.year
    );
}

shell_command sh_task(char* command) {
    task_t* current_task = g_task_list;
    printk(NONE, "ID  | Status\n");
    while (1) {
        list_t* next = current_task->list.next;
        printk(NONE, "%3d | %3d\n", current_task->id, current_task->status);
        if (next == NULL) {
            break;
        }
        current_task = container_of(next, task_t, list);
        if (current_task->id == 0) break;
    }
}

shell_command sh_readelf(char* command) {
    byte buffer[512] = {0};
    char* arg = strchr(command, ' ') + 1;

    struct elf_64* elf = read_elf(arg);

    kfree(elf);

    return 0;
}