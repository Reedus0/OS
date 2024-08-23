#pragma once

#include <stdint.h>
#include <stddef.h>

// Reference: https://www.cs.fsu.edu/~cop4610t/assignments/project3/spec/fatspec.pdf
// Reference: https://wiki.osdev.org/FAT

#include "include/fs.h"

enum FAT_TYPE {
    FAT12 = 0,
    FAT16 = 1,
    FAT32 = 2
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
    uint32_t file_size;
};

struct fat_data {
    enum FAT_TYPE fat_type;
    size_t fat_size;
    size_t fat_offset;
    size_t total_sectors;
    size_t total_root_dir_sectors;
    size_t root_dir_sector;
    size_t first_data_sector;
};

void fat_init(dev_t* dev, dir_t* root);
void fat_write_file(dev_t* dev, char* path, byte* buffer, size_t count);
void fat_read_file(dev_t* dev, char* path, byte* buffer, size_t count) ;
void fat_create_file(dev_t* dev, char* path);
void fat_delete_file(dev_t* dev, char* path);
void fat_create_dir(dev_t* dev, char* path);
void fat_delete_dir(dev_t* dev, char* path);

fs_t g_fs_fat = {
    .name = "FAT",
    .fs_data = NULL,

    .init = fat_init,

    .write_file = fat_write_file,
    .read_file = fat_read_file,
    .create_file = fat_create_file,
    .delete_file = fat_delete_file,

    .create_dir = fat_create_dir,
    .delete_dir = fat_delete_dir
};

