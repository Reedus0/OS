#pragma once

#include <stdint.h>
#include <stddef.h>

#include "include/list.h"
#include "include/dev.h"
#include "include/types.h"

struct file {

};
typedef struct file file_t;

struct vfs_file {
    char* name;
    void* fs_data;
    char* path;

    size_t position;
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
    void (*init)(vfs_fs_t* fs, vfs_dir_t* root);
    void (*deinit)(vfs_fs_t* fs, vfs_dir_t* root);

    void (*read_file)(vfs_fs_t* fs, vfs_file_t* file, byte* buffer, size_t count);
    void (*write_file)(vfs_fs_t* fs, vfs_file_t* file, byte* buffer, size_t count);
    vfs_file_t* (*create_file)(vfs_fs_t* fs, vfs_dir_t* dir, char* name);
    void (*delete_file)(vfs_fs_t* fs, vfs_dir_t* dir, char* name);

    vfs_dir_t* (*create_dir)(vfs_fs_t* fs, vfs_dir_t* parent, char* name);
    void (*delete_dir)(vfs_fs_t* fs, vfs_dir_t* parent, char* name);
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