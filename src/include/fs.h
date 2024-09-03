#pragma once

#include <stdint.h>
#include <stddef.h>

#include "include/list.h"
#include "include/dev.h"
#include "include/types.h"
#include "lib/string.h"

#define MAX_NAME_LENGTH 64
#define MAX_PATH_LENGTH 256

struct vfs_file {
    char* name;
    void* fs_data;
    char* path;

    size_t size;

    list_t list;
};
typedef struct vfs_file vfs_file_t;

struct vfs_dir {
    char* name;
    void* fs_data;

    list_t list;
    list_t subdirs;
    list_t files;
    struct vfs_dir* parent;
    bool mount_point;
};
typedef struct vfs_dir vfs_dir_t;

struct vfs_fs;
typedef struct vfs_fs vfs_fs_t;

struct vfs_func {
    void (*init)(vfs_fs_t* fs);
    void (*deinit)(vfs_fs_t* fs);

    size_t (*read_file)(vfs_fs_t* fs, vfs_file_t* file, byte* buffer, size_t offset, size_t count);
    size_t (*write_file)(vfs_fs_t* fs, vfs_file_t* file, byte* buffer, size_t offset, size_t count);
    vfs_file_t* (*create_file)(vfs_fs_t* fs, vfs_dir_t* dir, char* name);
    void (*delete_file)(vfs_fs_t* fs, vfs_file_t* file);

    vfs_dir_t* (*create_dir)(vfs_fs_t* fs, vfs_dir_t* dir, char* name);
    void (*delete_dir)(vfs_fs_t* fs, vfs_dir_t* dir);
};
typedef struct vfs_func vfs_func_t;

struct vfs_fs {
    void* fs_data;
    dev_t* dev;
    vfs_dir_t* mount_point;
    list_t list;

    vfs_func_t* func;
};
typedef struct vfs_fs vfs_fs_t;

struct file {
    vfs_file_t* vfs_file;
    size_t position;
};
typedef struct file file_t;

vfs_dir_t* vfs_new_dir(char* name, void* fs_data) {
    vfs_dir_t* new_dir = kalloc(sizeof(vfs_dir_t));

    new_dir->name = kalloc(MAX_NAME_LENGTH);
    strncpy(new_dir->name, name, MAX_NAME_LENGTH);
    new_dir->name = trim_string(new_dir->name);

    new_dir->fs_data = fs_data;

    return new_dir;
}

void vfs_remove_dir(vfs_dir_t* dir) {
    list_remove(&dir->list);

    kfree(dir->name);
    kfree(dir->fs_data);
    kfree(dir);
}

vfs_file_t* vfs_new_file(char* name, void* fs_data) {
    vfs_file_t* new_file = kalloc(sizeof(vfs_file_t));

    new_file->name = kalloc(MAX_NAME_LENGTH);
    strncpy(new_file->name, name, MAX_NAME_LENGTH);
    new_file->name = trim_string(new_file->name);

    new_file->fs_data = fs_data;

    return new_file;
}

void vfs_remove_file(vfs_file_t* file) {
    list_remove(&file->list);

    kfree(file->name);
    kfree(file->fs_data);
    kfree(file);
}

vfs_fs_t* vfs_new_fs(dev_t* dev, vfs_func_t* vfs_func) {
    vfs_fs_t* new_fs = kalloc(sizeof(vfs_fs_t));

    new_fs->dev = dev;
    new_fs->func = vfs_func;

    return new_fs;
}

void vfs_remove_fs(vfs_fs_t* fs) {
    list_remove(&fs->list);

    kfree(fs);
}

void vfs_add_dir(vfs_dir_t* root, vfs_dir_t* subdir) {
    list_t* last_list = &root->subdirs;

    while (last_list->next != NULL) {
        last_list = last_list->next;
    }
    list_insert_after(last_list, &subdir->list);

    subdir->parent = root;
}

void vfs_add_file(vfs_dir_t* root, vfs_file_t* file) {
    list_t* last_list = &root->files;
    while (last_list->next != NULL) {
        last_list = last_list->next;
    }
    list_insert_after(last_list, &file->list);
}