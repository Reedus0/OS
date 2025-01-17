#include "vfs.h"
#include "include/macro.h"
#include "include/dev.h"
#include "fs/fat/fs.h"

static void clean_files(vfs_dir_t* root) {
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

static void clean_dir(vfs_dir_t* root) {
    if (root->subdirs.next == NULL) {
        clean_files(root);
        return;
    }
    vfs_dir_t* current_dir = container_of(root->subdirs.next, vfs_dir_t, list);
    while (1) {
        list_t* next = current_dir->list.next;
        clean_dir(current_dir);
        if (next == NULL) {
            break;
        }
        current_dir = container_of(next, vfs_dir_t, list);
    }    
    clean_files(root);
}

void vfs_mount(vfs_dir_t* root, vfs_fs_t* fs) {
    fs->mount_point = root;
    fs->func->init(fs);

    if (g_fs_list != NULL) {
        list_insert_after(&g_fs_list->list, &fs->list);
    }
    g_fs_list = fs;
    
    root->mount_point = 1;
}

void vfs_umount(vfs_dir_t* root) {
    vfs_fs_t* current_fs = g_fs_list;

    while (1) {
        if (current_fs->mount_point == root) {
            current_fs->func->deinit(current_fs);

            clean_dir(root);
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

static vfs_dir_t* find_dir(vfs_dir_t* root, char* path) {
    vfs_dir_t* current_dir = root;
    while (*path != '\0') {
        if (current_dir->subdirs.next == NULL) {
            return current_dir;
        }
        current_dir = container_of(current_dir->subdirs.next, vfs_dir_t, list);
        if (strchr(path, '/')) path = strchr(path, '/') + 1;
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

static vfs_file_t* find_file(vfs_dir_t* dir, char* filename) {
    vfs_file_t* current_file = container_of(dir->files.next, vfs_file_t, list);

    if (current_file == NULL) {
        vfs_file_t* new_file = g_fs_list->func->create_file(g_fs_list, dir, filename);
        vfs_add_file(dir, new_file);
        return new_file;
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
    vfs_file_t* new_file = g_fs_list->func->create_file(g_fs_list, dir, filename);
    vfs_add_file(dir, new_file);
    return new_file;
}

file_t* vfs_open_file(char* path) {
    file_t* file = kalloc(sizeof(file_t));

    char* filename = strrchr(path, '/') + 1;
    if (filename == 1) filename = path;

    vfs_dir_t* file_dir = find_dir(&g_vfs_root, path);
    file->vfs_file = find_file(file_dir, filename);

    return file;
}

void vfs_close_file(file_t* file) {
    kfree(file);
}

void vfs_seek(file_t* file, size_t position) {
    file->position = position;
}

size_t vfs_read_file(file_t* file, byte* buffer, size_t count) {
    return g_fs_list->func->read_file(g_fs_list, file->vfs_file, buffer, file->position, count);
}

size_t vfs_write_file(file_t* file, byte* buffer, size_t count) {
    return g_fs_list->func->write_file(g_fs_list, file->vfs_file, buffer, file->position, count);
}

vfs_file_t* vfs_create_file(char* path) {
    vfs_dir_t* file_dir = find_dir(&g_vfs_root, path);
    char* filename = strrchr(path, '/') + 1;
    if (filename == 1) filename = path;

    vfs_file_t* new_file = g_fs_list->func->create_file(g_fs_list, file_dir, filename);
    vfs_add_file(file_dir, new_file);

    return new_file;
}

void vfs_delete_file(char* path) {
    vfs_dir_t* file_dir = find_dir(&g_vfs_root, path);
    char* filename = strrchr(path, '/') + 1;
    if (filename == 1) filename = path;

    vfs_file_t* file = find_file(file_dir, filename);
    
    g_fs_list->func->delete_file(g_fs_list, file);
    vfs_remove_file(file);
}

vfs_dir_t* vfs_create_dir(char* path) {
    vfs_dir_t* parent_dir = find_dir(&g_vfs_root, path);
    char* dirname = strrchr(path, '/') + 1;
    if (dirname == 1) dirname = path;

    vfs_file_t* new_dir = g_fs_list->func->create_dir(g_fs_list, parent_dir, dirname);
    vfs_add_dir(parent_dir, new_dir);

    return new_dir;
}

void vfs_delete_dir(char* path) {
    vfs_dir_t* dir = find_dir(&g_vfs_root, path);

    g_fs_list->func->delete_dir(g_fs_list, dir);
    vfs_remove_dir(dir);
}

void init_vfs() {
    printk(INFO, "Initiating VFS...\n");
    g_vfs_root.name = "/";
    g_vfs_root.parent = &g_vfs_root;

    vfs_fs_t* fat = vfs_new_fs(&g_hdd, &g_vfs_func_fat);

    vfs_mount(&g_vfs_root, fat);   

    printk(SUCCESS, "Initiated VFS!\n"); 
}