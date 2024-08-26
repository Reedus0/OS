#pragma once

#include <stdint.h>
#include <stddef.h>

#include "include/list.h"
#include "include/dev.h"
#include "include/types.h"

struct file {
    char* name;
    void* fs_data;
    char* path;

    size_t position;
    size_t size;

    list_t list;
};
typedef struct file file_t;

struct dir {
    char* name;
    void* fs_data;

    list_t list;
    list_t subdirs;
    list_t files;
    struct dir* parent;
    bool mount_point;
};
typedef struct dir dir_t;

struct fs;
typedef struct fs fs_t;

struct fs {
    char* name;
    void* fs_data;

    void (*init)(fs_t* fs, dev_t* dev, dir_t* root);
    void (*deinit)(fs_t* fs, dev_t* dev, dir_t* root);

    void (*read_file)(fs_t* fs, dev_t* dev, file_t* file, byte* buffer, size_t count);
    void (*write_file)(fs_t* fs, dev_t* dev, file_t* file, byte* buffer, size_t count);
    file_t* (*create_file)(fs_t* fs, dev_t* dev, dir_t* dir, char* name);
    void (*delete_file)(fs_t* fs, dev_t* dev, dir_t* dir, char* name);

    dir_t* (*create_dir)(fs_t* fs, dev_t* dev, dir_t* parent, char* name);
    void (*delete_dir)(fs_t* fs, dev_t* dev, dir_t* parent, char* name);
};
typedef struct fs fs_t;