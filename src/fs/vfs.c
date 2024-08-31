#include "vfs.h"
#include "include/macro.h"
#include "include/dev.h"
#include "fs/fat/fs.h"

void vfs_mount(vfs_dir_t* root, vfs_fs_t* fs) {
    fs->mount_point = root;
    fs->func->init(fs);

    if (g_fs_list != NULL) {
        list_insert_after(&g_fs_list->list, &fs->list);
    }
    g_fs_list = fs;
    
    root->mount_point = 1;
}

void vfs_clean_files(vfs_dir_t* root) {
    if (root->files.next == NULL) {
        if (root != &g_vfs_root) {
            vfs_remove_dir(root);
        }
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
        vfs_remove_dir(root);
    }
    return;
}

void vfs_clean_dir(vfs_dir_t* root) {
    if (root->subdirs.next == NULL) {
        vfs_clean_files(root);
        return;
    }
    vfs_dir_t* current_dir = container_of(root->subdirs.next, vfs_dir_t, list);
    while (1) {
        list_t* next = current_dir->list.next;
        vfs_clean_dir(current_dir);
        if (next == NULL) {
            break;
        }
        current_dir = container_of(next, vfs_dir_t, list);
    }    
    vfs_clean_files(root);
}

void vfs_umount(vfs_dir_t* root) {
    vfs_fs_t* current_fs = g_fs_list;

    while (1) {
        if (current_fs->mount_point == root) {
            current_fs->func->deinit(current_fs);

            vfs_clean_dir(root);
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

void vfs_read_file(vfs_file_t* file, byte* buffer, size_t offset, size_t count) {
    g_fs_list->func->read_file(g_fs_list, file, buffer, offset, count);
}

void vfs_write_file(vfs_file_t* file, byte* buffer, size_t offset, size_t count) {
    g_fs_list->func->write_file(g_fs_list, file, buffer, offset, count);
}

void vfs_delete_file(vfs_file_t* file) {
    g_fs_list->func->delete_file(g_fs_list, file);
}

void vfs_add_subdir(vfs_dir_t* root, vfs_dir_t* subdir) {
    list_t* last_list = &root->subdirs;

    while (last_list->next != NULL) {
        last_list = last_list->next;
    }
    list_insert_after(last_list, &subdir->list);

    subdir->parent = root;
}

void vfs_add_file(vfs_dir_t* root, vfs_file_t* file) {
    list_t* last_list = &root->files;
    while (last_list->next != NULL) {
        last_list = last_list->next;
    }
    list_insert_after(last_list, &file->list);
}

void init_vfs() {
    g_vfs_root.name = "/";
    g_vfs_root.parent = &g_vfs_root;

    vfs_fs_t* fat = vfs_create_fs(&g_hdd, &g_vfs_func_fat);

    vfs_mount(&g_vfs_root, fat);    

    vfs_dir_t* file_dir = vfs_find_dir(&g_vfs_root, "/dir");
    // vfs_create_file("root", );
    // vfs_file_t* file = vfs_find_file(file_dir, "dir_file");
    // vfs_write_file(file, hello, 0, 12);
    // vfs_read_file(file, buffer, 0, 12);
    // printk("%s", buffer);
    // vfs_delete_file(file);

    // vfs_umount(&g_vfs_root);
    // vfs_remove_fs(fat);
    // kalloc(1);
}