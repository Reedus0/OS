#include "vfs.h"

void init_vfs() {
    g_root_fdata.id = 0;

    g_vfs_root.fdata = &g_root_fdata;
    g_vfs_root.name = "/";
    g_vfs_root.parent = &g_vfs_root;
}

void vfs_mount(dir_t* root, vfs_entry_t* vfs_entry) {
    root->mount_point = 1;
}

void vfs_umount(dir_t* root) {
    root->mount_point = 0;
}

dir_t* vfs_find_closest_mounted(char* path) {

}

vfs_entry_t* vfs_get_mounted_vfs_entry(dir_t* entry) {

}

char* vfs_get_mounted_path(dir_t* mounted, char* path) {
    
}


void vfs_write_file(file_t* file, byte* buffer, size_t count) {
    // dir_t* mounted = vfs_find_closest_mounted(file->path); - search for parent mounted dir
    // vfs_entry_t* vfs_entry = vfs_get_mounted_entry(mounted); - get vfs_entry
    // char* mounted_path = vfs_get_mounted_path(mounted, file->path);
    // vfs_entry->fs->write_file(vfs_entry->dev, mounted_path, buffer, count);
}

void vfs_read_file(file_t* file, byte* buffer, size_t count) {
    // dir_t* mounted = vfs_find_closest_mounted(file->path);
    // vfs_entry_t* vfs_entry = vfs_get_mounted_entry(mounted);
    // char* mounted_path = vfs_get_mounted_path(mounted, file->path);
    // vfs_entry->fs->read_file(vfs_entry->dev, mounted_path, buffer, count);
}

void vfs_create_file(dir_t* root, char* name) {
    // vfs_entry_t* vfs_entry = vfs_get_mounted_entry(root); 
    // char* mounted_path = vfs_get_mounted_path(mounted, file->path);
    // vfs_entry->fs->create_file(vfs_entry->dev, mounted_path, name);
}

void vfs_delete_file(file_t* file) {
    // dir_t* mounted = vfs_find_closest_mounted(file->path);
    // vfs_entry_t* vfs_entry = vfs_get_mounted_entry(mounted);
    // char* mounted_path = vfs_get_mounted_path(mounted, file->path);
    // vfs_entry->fs->delete_file(vfs_entry->dev, mounted_path);
}

void vfs_create_dir(dir_t* root, char* name) {
    // vfs_entry_t* vfs_entry = vfs_get_mounted_entry(mounted);
    // char* mounted_path = vfs_get_mounted_path(mounted, file->path);
    // vfs_entry->fs->create_dir(vfs_entry->dev, mounted_path, name);
}

void vfs_delete_dir(dir_t* dir) {
    // vfs_entry_t* vfs_entry = vfs_get_mounted_entry(mounted);
    // char* mounted_path = vfs_get_mounted_path(mounted, file->path);
    // vfs_entry->fs->delete_dir(vfs_entry->dev, mounted_path);
}

void vfs_add_subdir(dir_t* root, dir_t* subdir) {
    list_t* last_list = &root->subdirs;
    while (last_list->next != NULL) {
        last_list = root->subdirs.next;
    }
    list_insert_after(last_list, &subdir->subdirs);
    
    subdir->parent = root;
}

void vfs_add_file(dir_t* root, file_t* file) {
    list_t* last_list = &root->files;
    while (last_list->next != NULL) {
        last_list = root->files.next;
    }
    list_insert_after(last_list, &file->list);
}