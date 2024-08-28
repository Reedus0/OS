#pragma once

#include "include/fs.h"
#include "include/list.h"

dir_t g_vfs_root;

fs_t* g_fs_list;

void init_vfs();