#include "vfs.h"

void init_vfs() {
    g_root_fdata.id = 0;

    g_vfs_root.fdata = &g_root_fdata;
    g_vfs_root.name = "/";
    g_vfs_root.parent = &g_vfs_root;

    fdata_t* folder_fdata = kalloc(sizeof(fdata_t));
    dir_t* folder = kalloc(sizeof(dir_t));

    folder_fdata->id = 1;

    folder->fdata = folder_fdata;
    folder->name = "folder";
    vfs_add_subdir(&g_vfs_root, folder);
}

void vfs_add_subdir(dir_t* root, dir_t* subdir) {
    list_t* last_list = &root->subdirs;
    while (last_list->next != NULL) {
        last_list = root->subdirs.next;
    }
    list_insert_after(last_list, &subdir->subdirs);
    
    subdir->parent = root;
}