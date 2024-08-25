#pragma once

#include <stdint.h>
#include <stddef.h>

#include "include/list.h"
#include "include/dev.h"
#include "include/types.h"

struct file {
    char* name;
    char* path;
    list_t list;
};
typedef struct file file_t;

struct dir {
    char* name;
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

    void (*write_file)(fs_t* fs, dev_t* dev, char* path, byte* buffer, size_t count);
    void (*read_file)(fs_t* fs, dev_t* dev, char* path, byte* buffer, size_t count);
    void (*create_file)(fs_t* fs, dev_t* dev, char* path);
    void (*delete_file)(fs_t* fs, dev_t* dev, char* path);

    void (*create_dir)(fs_t* fs, dev_t* dev, char* path);
    void (*delete_dir)(fs_t* fs, dev_t* dev, char* path);
};
typedef struct fs fs_t;