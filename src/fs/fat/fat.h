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

struct fat_file {
    fat_entry_t* entry;
    list_t list;
};

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

struct fat_data {
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
    byte* fats;
};

void init(dev_t* dev, dir_t* root);
void write_file(dev_t* dev, char* path, byte* buffer, size_t count);
void read_file(dev_t* dev, char* path, byte* buffer, size_t count) ;
void create_file(dev_t* dev, char* path);
void delete_file(dev_t* dev, char* path);
void create_dir(dev_t* dev, char* path);
void delete_dir(dev_t* dev, char* path);

fs_t g_fs_fat = {
    .name = "FAT",
    .fs_data = NULL,

    .init = init,

    .write_file = write_file,
    .read_file = read_file,
    .create_file = create_file,
    .delete_file = delete_file,

    .create_dir = create_dir,
    .delete_dir = delete_dir
};

