#pragma once

#include "include/fs.h"
#include "include/list.h"

vfs_dir_t g_vfs_root;

vfs_fs_t* g_fs_list;

void init_vfs();