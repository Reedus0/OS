#pragma once

#include <stdint.h>
#include <stddef.h>

// Reference: https://www.cs.fsu.edu/~cop4610t/assignments/project3/spec/fatspec.pdf
// Reference: https://wiki.osdev.org/FAT

#include "include/fs.h"

#define FIRST_DATA_CLUSTER 5
#define ROOT_CLUSTER 0

struct fat_file_data {
    uint32_t cluster;
    uint32_t dir_cluster;
};
typedef struct fat_file_data fat_file_data_t;

vfs_fs_t* create_fs_fat(dev_t* dev);
void delete_fs_fat(vfs_fs_t* fs_fat);

void init(vfs_fs_t* fs);
void deinit(vfs_fs_t* fs);

size_t read_file(vfs_fs_t* fs, vfs_file_t* file, byte* buffer, size_t offset, size_t count);
size_t write_file(vfs_fs_t* fs, vfs_file_t* file, byte* buffer, size_t offset, size_t count);
vfs_file_t* create_file(vfs_fs_t* fs, vfs_dir_t* dir, char* name);
void delete_file(vfs_fs_t* fs, vfs_file_t* file);

vfs_dir_t* create_dir(vfs_fs_t* fs, vfs_dir_t* dir, char* name);
void delete_dir(vfs_fs_t* fs, vfs_dir_t* dir);

vfs_func_t g_vfs_func_fat = {
    .init = init,
    .deinit = deinit,

    .read_file = read_file,
    .write_file = write_file,
    .create_file = create_file,
    .delete_file = delete_file,

    .create_dir = create_dir,
    .delete_dir = delete_dir
};