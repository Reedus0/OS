#include "vfs.h"
#include "include/macro.h"
#include "include/dev.h"
#include "lib/string.h"
#include "fs/fat/fat.h"

void vfs_mount(dir_t* root, fs_t* fs) {
    fs->mount_point = root;
    fs->func->init(fs, root);

    if (g_fs_list != NULL) {
        list_insert_after(&g_fs_list->list, &fs->list);
    }
    g_fs_list = fs;
    
    root->mount_point = 1;
}

void vfs_umount(dir_t* root) {
    root->mount_point = 0;

    fs_t* current_fs = g_fs_list;

    while (1) {
        if (current_fs->mount_point == root) {
            current_fs->func->deinit(current_fs, root);
            list_remove(current_fs);
            return;
        }
        if (current_fs->list.prev == NULL) {
            break;
        }
        current_fs = container_of(current_fs->list.prev, fs_t, list);
    }
}

dir_t* vfs_find_dir(char* path) {
    dir_t* current_dir = &g_vfs_root;
    while (*path != '\0') {
        path = strchr(path, '/');
        while (1) {
            size_t current_dir_name_length = strlen(current_dir->name);
            if (strncmp(current_dir->name, path, current_dir_name_length)) {
                path += current_dir_name_length;
                break;
            }
            if (current_dir->subdirs.next == NULL) {
                return current_dir->parent;
            }
            current_dir = container_of(current_dir->subdirs.next, dir_t, list);
        }
    }
    return current_dir;
}

file_t* vfs_find_file(dir_t* dir, char* filename) {
    file_t* current_file = container_of(dir->files.next, file_t, list);
    if (current_file == NULL) {
        return g_fs_list->func->create_file(g_fs_list, dir, filename);
    }
    while (1) {
        if (strcmp(current_file->name, filename)) {
            return current_file;
        }
        if (current_file->list.next == NULL) {
            break;
        }
        current_file = container_of(current_file->list.next, file_t, list);
    }
    return g_fs_list->func->create_file(g_fs_list, dir, filename);
}

void vfs_seek(file_t* file, size_t offset) {
    file->position = offset;
}

void vfs_read_file(file_t* file, byte* buffer, size_t count) {
    g_fs_list->func->read_file(g_fs_list, file, buffer, count);
}
void vfs_write_file(file_t* file, byte* buffer, size_t count) {
    g_fs_list->func->write_file(g_fs_list, file, buffer, count);
}

void vfs_delete_file(dir_t* dir, char* name) {
}

void vfs_create_dir(dir_t* parent, char* name) {
}

void vfs_delete_dir(dir_t* parent, char* name) {
}

void vfs_add_subdir(dir_t* root, dir_t* subdir) {
    list_t* last_list = &root->subdirs;

    while (last_list->next != NULL) {
        last_list = last_list->next;
    }
    list_insert_after(last_list, &subdir->list);
    
    subdir->parent = root;
}

void vfs_remove_subdir(dir_t* root, dir_t* subdir) {
    list_remove(&subdir->list);

    kfree(subdir->name);
    kfree(subdir);
}

void vfs_add_file(dir_t* root, file_t* file) {
    list_t* last_list = &root->files;
    while (last_list->next != NULL) {
        last_list = last_list->next;
    }
    list_insert_after(last_list, &file->list);
}

void vfs_remove_file(file_t* file) {
    list_remove(&file->list);

    kfree(file->name);
    kfree(file);
}

dir_t* vfs_new_dir(char* name, void* fs_data) {
    dir_t* new_dir = kalloc(sizeof(dir_t));

    new_dir->name = kalloc(64);
    strncpy(new_dir->name, name, strlen(name));
    new_dir->name[strlen(new_dir->name) - 1] = 0;

    new_dir->files.next = NULL;
    new_dir->files.prev = NULL;

    new_dir->subdirs.next = NULL;
    new_dir->subdirs.prev = NULL;

    new_dir->fs_data = fs_data;

    return new_dir;
}

file_t* vfs_new_file(char* name, void* fs_data) {
    file_t* new_file = kalloc(sizeof(file_t));

    new_file->name = kalloc(64);
    strncpy(new_file->name, name, strlen(name));
    new_file->name[strlen(new_file->name) - 1] = 0;

    new_file->fs_data = fs_data;
    new_file->position = 0;

    return new_file;
}

void init_vfs() {

    byte buffer[] = "CCCCCCCCC";
    byte buffer2[] = "BBBBBBBBBB";

    g_vfs_root.name = "/";
    g_vfs_root.parent = &g_vfs_root;

    fs_t* fat = create_fs_fat(&g_hdd);
    
    vfs_mount(&g_vfs_root, fat);    

    dir_t* file_dir = vfs_find_dir("/");
    file_t* file = vfs_find_file(file_dir, "FILE       ");

    printk("file: %s\n", file->name);

    vfs_seek(file, 0);
    vfs_write_file(file, buffer2, 9);
    vfs_seek(file, 2048);
    vfs_write_file(file, buffer2, 9);
    // vfs_seek(file, 4096);
    // vfs_write_file(file, buffer2, 9);
    // vfs_seek(file, 6144);
    // vfs_write_file(file, buffer2, 9);

    //dir_t* file_dir = vfs_find_dir("/");

    //vfs_delete_file(file_dir, "FILE       ");
}