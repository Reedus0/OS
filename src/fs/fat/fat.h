#pragma once

#include <stdint.h>
#include <stddef.h>

// Reference: https://www.cs.fsu.edu/~cop4610t/assignments/project3/spec/fatspec.pdf
// Reference: https://wiki.osdev.org/FAT

#include "include/fs.h"

#define FIRST_DATA_CLUSTER 5

enum FAT_ATTRIBUTES {
    READ_ONLY = 0x1,
    HIDDEN = 0x2,
    SYSTEM = 0x4,
    VALUME_ID = 0x8,
    DIRECTORY = 0x10,
    ARCHIVE = 0x20,
    LFN = 0xF,
};

struct fat_entry {
    uint8_t name[11];
    uint8_t attributes;
    uint8_t reserved_1;
    uint8_t creation_mills;
    uint16_t creation_time;
    uint16_t creation_date;
    uint16_t access_date;
    uint16_t cluster_high;
    uint16_t modification_time;
    uint16_t modification_date;
    uint16_t cluster_low;
    uint32_t size;
} __attribute__((packed));
typedef struct fat_entry fat_entry_t;

struct fat_long_name {
    uint8_t order;
    uint8_t name_1[10];
    uint8_t attribute;
    uint8_t entry_type;
    uint8_t checksum;
    uint8_t name_2[12];
    uint8_t reserved_1;
    uint8_t name_3[2];
} __attribute__((packed));
typedef struct fat_long_name fat_long_name_t;

struct fat_file_data {
    size_t cluster;
    size_t dir_cluster;
};
typedef struct fat_file_data fat_file_data_t;

vfs_fs_t* create_fs_fat(dev_t* dev);
void delete_fs_fat(vfs_fs_t* fs_fat);

void init(vfs_fs_t* fs, vfs_dir_t* root);
void deinit(vfs_fs_t* fs, vfs_dir_t* root);

void read_file(vfs_fs_t* fs, vfs_file_t* file, byte* buffer, size_t count);
void write_file(vfs_fs_t* fs, vfs_file_t* file, byte* buffer, size_t count);
vfs_file_t* create_file(vfs_fs_t* fs, vfs_dir_t* dir, char* name);
void delete_file(vfs_fs_t* fs, vfs_dir_t* dir, char* name);

vfs_dir_t* create_dir(vfs_fs_t* fs, vfs_dir_t* parent, char* name);
void delete_dir(vfs_fs_t* fs, vfs_dir_t* parent, char *name);

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