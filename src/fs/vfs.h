#pragma once

#include "include/file.h"
#include "include/list.h"

struct dir {
    char* name;
    fdata_t* fdata; 
    struct dir* parent;
    list_t subdirs;
    list_t files;
};
typedef struct dir dir_t;

fdata_t g_root_fdata;
dir_t g_vfs_root;

void init_vfs();