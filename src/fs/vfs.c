#include "vfs.h"
#include "include/macro.h"
#include "include/dev.h"
#include "lib/string.h"
#include "fs/fat/fat.h"

void vfs_mount(dir_t* root, vfs_entry_t* vfs_entry) {
    vfs_entry->mount_point = root;
    vfs_entry->fs->init(vfs_entry->fs, vfs_entry->dev, root);

    if (g_vfs_entries != NULL) {
        list_insert_after(&g_vfs_entries->list, &vfs_entry->list);
    }
    
    root->mount_point = 1;
    g_vfs_entries = vfs_entry;
}

void vfs_umount(dir_t* root) {
    root->mount_point = 0;

    vfs_entry_t* current_vfs_entry = g_vfs_entries;

    while (1) {
        if (current_vfs_entry->mount_point == root) {
            current_vfs_entry->fs->deinit(current_vfs_entry->fs, current_vfs_entry->dev, root);
            list_remove(current_vfs_entry);
            return;
        }
        if (current_vfs_entry->list.prev == NULL) {
            break;
        }
        current_vfs_entry = container_of(current_vfs_entry->list.prev, vfs_entry_t, list);
    }
}

// dir_t* vfs_find_dir(char* path) {
//     dir_t* current_dir = &g_vfs_root;
//     while (*path != '\0') {
//         path = strchr(path, '/');
//         while (1) {
//             size_t current_dir_name_length = strlen(current_dir->name);
//             if (strncmp(current_dir->name, path, current_dir_name_length)) {
//                 path += current_dir_name_length;
//                 break;
//             }
//             if (current_dir->subdirs.next == NULL) {
//                 break;
//             }
//             current_dir = container_of(current_dir->subdirs.next, dir_t, subdirs);
//         }
//     }
//     return current_dir;
// }

// file_t* vfs_open_file(char* path) {
//     dir_t* file_dir = vfs_find_dir(path);
//     file_t* current_file = file_dir->files;
//     char* filename = strrchr(path, '/');
//     while (1) {
//         if (strcmp(current_file->name, filename)) {
//             return current_file;
//         }
//         if (current_file->list.next == NULL) {
//             break;
//         }
//         current_file = container_of(current_file->list.next, file_t, list);
//     }
//     return NULL;
// }

void vfs_write_file(file_t* file, byte* buffer, size_t count) {
    g_vfs_entries->fs->write_file(g_vfs_entries->fs, g_vfs_entries->dev, file->path, buffer, count);
}

void vfs_read_file(file_t* file, byte* buffer, size_t count) {
    g_vfs_entries->fs->read_file(g_vfs_entries->fs, g_vfs_entries->dev, file->path, buffer, count);
}

void vfs_delete_file(char* path) {
    g_vfs_entries->fs->delete_file(g_vfs_entries->fs, g_vfs_entries->dev, path);
}

void vfs_create_dir(char* path) {
    g_vfs_entries->fs->create_dir(g_vfs_entries->fs, g_vfs_entries->dev, path);
}

void vfs_delete_dir(char* path) {
    g_vfs_entries->fs->delete_dir(g_vfs_entries->fs, g_vfs_entries->dev, path);
}

void vfs_add_subdir(dir_t* root, dir_t* subdir) {
    list_t* last_list = &root->subdirs;

    while (last_list->next != NULL) {
        last_list = last_list->next;
    }
    list_insert_after(last_list, &subdir->list);
    
    subdir->parent = root;
}

void vfs_add_file(dir_t* root, file_t* file) {
    list_t* last_list = &root->files;
    while (last_list->next != NULL) {
        last_list = last_list->next;
    }
    list_insert_after(last_list, &file->list);
}

dir_t* vfs_new_dir(char* name) {
    dir_t* new_dir = kalloc(sizeof(dir_t));

    new_dir->name = kalloc(64);
    strncpy(new_dir->name, name, strlen(name));
    new_dir->name[strlen(new_dir->name) - 1] = 0;

    new_dir->files.next = NULL;
    new_dir->files.prev = NULL;

    new_dir->subdirs.next = NULL;
    new_dir->subdirs.prev = NULL;

    return new_dir;
}

file_t* vfs_new_file(char* name) {
    file_t* new_file = kalloc(sizeof(file_t));

    new_file->name = kalloc(64);
    strncpy(new_file->name, name, strlen(name));
    new_file->name[strlen(new_file->name) - 1] = 0;

    return new_file;
}

void init_vfs() {

    byte buffer[512];

    g_vfs_root.name = "/";
    g_vfs_root.parent = &g_vfs_root;

    g_vfs_fat.dev = &g_hdd;
    g_vfs_fat.fs = &g_fs_fat;
    
    vfs_mount(&g_vfs_root, &g_vfs_fat);

    g_vfs_entries->fs->read_file(g_vfs_entries->fs, g_vfs_entries->dev, "/test", buffer, 100);
}