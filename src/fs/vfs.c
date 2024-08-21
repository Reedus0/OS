#include "vfs.h"

#include "include/kalloc.h"

void init_vfs() {
    g_root_fdata.id = 0;

    g_vfs_root.fdata = &g_root_fdata;
    g_vfs_root.name = "/";
    g_vfs_root.parent = &g_vfs_root;
}