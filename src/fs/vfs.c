#include "vfs.h"
#include "include/macro.h"
#include "include/dev.h"
#include "lib/string.h"
#include "fs/fat/fat.h"

void vfs_mount(vfs_dir_t* root, vfs_fs_t* fs) {
    fs->mount_point = root;
    fs->func->init(fs, root);

    if (g_fs_list != NULL) {
        list_insert_after(&g_fs_list->list, &fs->list);
    }
    g_fs_list = fs;
    
    root->mount_point = 1;
}

void vfs_clean_subdir(vfs_dir_t* root) {
    if (root->files.next == NULL) {
        vfs_remove_subdir(root);
        return;
    }
    vfs_file_t* current_file = container_of(root->files.next, vfs_file_t, list);
    while (1) {
        list_t* next = current_file->list.next;
        vfs_remove_file(current_file);
        if (next == NULL) {
            break;
        }
        current_file = container_of(next, vfs_file_t, list);
    }
    if (root != &g_vfs_root) {
        vfs_remove_subdir(root);
    }
    return;
}

void vfs_clean_mount(vfs_dir_t* root) {
    if (root->subdirs.next == NULL) {
        vfs_clean_subdir(root);
        return;
    }
    vfs_dir_t* current_dir = container_of(root->subdirs.next, vfs_dir_t, list);
    while (1) {
        list_t* next = current_dir->list.next;
        vfs_clean_mount(current_dir);
        if (next == NULL) {
            break;
        }
        current_dir = container_of(next, vfs_dir_t, list);
    }    
    vfs_clean_subdir(root);
}

void vfs_umount(vfs_dir_t* root) {
    vfs_fs_t* current_fs = g_fs_list;

    while (1) {
        if (current_fs->mount_point == root) {
            current_fs->func->deinit(current_fs, root);

            vfs_clean_mount(root);
            list_remove(current_fs);
            return;
        }
        if (current_fs->list.prev == NULL) {
            break;
        }
        current_fs = container_of(current_fs->list.prev, vfs_fs_t, list);
    }
    root->mount_point = 0;
}

vfs_dir_t* vfs_find_dir(vfs_dir_t* root, char* path) {
    vfs_dir_t* current_dir = root;
    while (*path != '\0') {
        current_dir = container_of(current_dir->subdirs.next, vfs_dir_t, list);
        path = strchr(path, '/') + 1;
        while (1) {
            size_t current_dir_name_length = strlen(current_dir->name);
            if (strncmp(current_dir->name, path, current_dir_name_length)) {
                path += current_dir_name_length;
                break;
            }
            if (current_dir->list.next == NULL) {
                return current_dir->parent;
            }
            current_dir = container_of(current_dir->list.next, vfs_dir_t, list);
        }
    }
    return current_dir;
}

vfs_file_t* vfs_find_file(vfs_dir_t* dir, char* filename) {
    vfs_file_t* current_file = container_of(dir->files.next, vfs_file_t, list);
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
        current_file = container_of(current_file->list.next, vfs_file_t, list);
    }
    return g_fs_list->func->create_file(g_fs_list, dir, filename);
}

void vfs_seek(vfs_file_t* file, size_t offset) {
    file->position = offset;
}

void vfs_read_file(vfs_file_t* file, byte* buffer, size_t count) {
    g_fs_list->func->read_file(g_fs_list, file, buffer, count);
}

void vfs_write_file(vfs_file_t* file, byte* buffer, size_t count) {
    g_fs_list->func->write_file(g_fs_list, file, buffer, count);
}

void vfs_delete_file(vfs_file_t* file) {
    g_fs_list->func->delete_file(g_fs_list, file);
}

void vfs_create_dir(vfs_dir_t* parent, char* name) {
}

void vfs_delete_dir(vfs_dir_t* dir) {
     g_fs_list->func->delete_dir(g_fs_list, dir);
}

vfs_dir_t* vfs_new_dir(char* name, void* fs_data) {
    vfs_dir_t* new_dir = kalloc(sizeof(vfs_dir_t));

    new_dir->name = kalloc(64);
    strncpy(new_dir->name, name, 64);
    new_dir->name = trim_string(new_dir->name);

    new_dir->files.next = NULL;
    new_dir->files.prev = NULL;

    new_dir->subdirs.next = NULL;
    new_dir->subdirs.prev = NULL;

    new_dir->fs_data = fs_data;

    return new_dir;
}

vfs_file_t* vfs_new_file(char* name, void* fs_data) {
    vfs_file_t* new_file = kalloc(sizeof(vfs_file_t));

    new_file->name = kalloc(64);
    strncpy(new_file->name, name, 64);
    new_file->name = trim_string(new_file->name);

    new_file->fs_data = fs_data;
    new_file->position = 0;

    return new_file;
}

void vfs_add_subdir(vfs_dir_t* root, vfs_dir_t* subdir) {
    list_t* last_list = &root->subdirs;

    while (last_list->next != NULL) {
        last_list = last_list->next;
    }
    list_insert_after(last_list, &subdir->list);

    subdir->parent = root;
}

void vfs_remove_subdir(vfs_dir_t* subdir) {
    list_remove(&subdir->list);

    kfree(subdir->name);
    kfree(subdir->fs_data);
    kfree(subdir);
}

void vfs_add_file(vfs_dir_t* root, vfs_file_t* file) {
    list_t* last_list = &root->files;
    while (last_list->next != NULL) {
        last_list = last_list->next;
    }
    list_insert_after(last_list, &file->list);
}

void vfs_remove_file(vfs_file_t* file) {
    list_remove(&file->list);

    kfree(file->name);
    kfree(file->fs_data);
    kfree(file);
}

void init_vfs() {
    g_vfs_root.name = "/";
    g_vfs_root.parent = &g_vfs_root;

    vfs_fs_t* fat = create_fs_fat(&g_hdd);

    vfs_mount(&g_vfs_root, fat);    

    byte buffer[] = "CCCCCCCCC";
    byte buffer2[] = "BBBBBBBBBB";

    vfs_dir_t* file_dir = vfs_find_dir(&g_vfs_root, "/dir");
    vfs_file_t* file = vfs_find_file(file_dir, "dir_file");
    // vfs_write_file(file, buffer2, 4);
    // vfs_read_file(file, buffer, 4);
    // printk("%s", file_dir->name);

    // vfs_delete_file(file);

    // vfs_umount(&g_vfs_root);
    // delete_fs_fat(fat);
    // kalloc(1);
}