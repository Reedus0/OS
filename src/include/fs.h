#pragma once

#include <stdint.h>
#include <stddef.h>

#include "include/list.h"
#include "include/dev.h"
#include "include/types.h"

struct fdata {
    size_t id;
};
typedef struct fdata fdata_t;

struct file {
    fdata_t* fdata;
    char* name;
    char* path;
    list_t list;
};
typedef struct file file_t;

struct dir {
    char* name;
    fdata_t* fdata; 
    struct dir* parent;
    bool mount_point;
    list_t subdirs;
    list_t files;
};
typedef struct dir dir_t;

struct fs {
    char* name;
};
typedef struct fs fs_t;