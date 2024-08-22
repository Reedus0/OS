#pragma once

#include "include/fs.h"
#include "include/list.h"

struct vfs_entry;
typedef struct vfs_entry vfs_entry_t;

struct vfs_entry {
    dir_t* mount_point;
    fs_t* fs;
    dev_t* dev;
    list_t list;
};
typedef struct vfs_entry vfs_entry_t;

fdata_t g_root_fdata;
dir_t g_vfs_root;

void init_vfs();