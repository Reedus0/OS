#pragma once

#include <stdint.h>
#include <stddef.h>

// Reference: https://www.cs.fsu.edu/~cop4610t/assignments/project3/spec/fatspec.pdf
// Reference: https://wiki.osdev.org/FAT

#include "include/fs.h"

#define FIRST_DATA_CLUSTER 5

enum FAT_TYPE {
    FAT12 = 0,
    FAT16 = 1,
    FAT32 = 2
};

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
typedef struct fat_entry fat_file_t;
typedef struct fat_entry fat_dir_t;

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

struct fat_info {
    enum FAT_TYPE fat_type;
    size_t fat_size;
    size_t total_sectors;
    size_t total_root_dir_sectors;
    size_t total_fats;
    size_t fat_region;
    size_t root_dir_region;
    size_t data_region;
    size_t sectors_per_claster;
    size_t sector_size;
    size_t cluster_size;
    size_t eof;
    byte* fats;
};

struct fat_file_data {
    size_t cluster;
};
typedef struct fat_file_data fat_file_data_t;

struct fat_data {
    void* entry;
    list_t list;
};
typedef struct fat_data fat_data_t;

void init(fs_t* fs, dev_t* dev, dir_t* root);
void deinit(fs_t* fs, dev_t* dev, dir_t* root);

void read_file(fs_t* fs, dev_t* dev, file_t* file, byte* buffer, size_t count);
void write_file(fs_t* fs, dev_t* dev, file_t* file, byte* buffer, size_t count);
file_t* create_file(fs_t* fs, dev_t* dev, dir_t* dir, char* name);
void delete_file(fs_t* fs, dev_t* dev, dir_t* dir, char* name);

dir_t* create_dir(fs_t* fs, dev_t* dev, dir_t* parent, char* name);
void delete_dir(fs_t* fs, dev_t* dev, dir_t* parent, char *name);

fs_t g_fs_fat = {
    .name = "FAT",
    .fs_data = NULL,

    .init = init,
    .deinit = deinit,

    .read_file = read_file,
    .write_file = write_file,
    .create_file = create_file,
    .delete_file = delete_file,

    .create_dir = create_dir,
    .delete_dir = delete_dir
};