#pragma once

#include <stdint.h>
#include <stddef.h>

#include "fs.h"
#include "cluster.h"
#include "entry.h"
#include "include/dev.h"

enum FAT_TYPE {
    FAT12 = 0,
    FAT16 = 1,
    FAT32 = 2
};

struct fat_info {
    enum FAT_TYPE fat_type;
    size_t fat_size;
    size_t total_root_dir_sectors;
    size_t total_fats;
    size_t fat_region;
    size_t root_dir_region;
    size_t data_region;
    size_t sectors_per_claster;
    size_t sector_size;
    size_t cluster_size;
    size_t eof;
};

struct fat_info* fat_get_info(vfs_fs_t* fs);
void fat_parse_dir(vfs_fs_t* fs, fat_entry_t* dir, vfs_dir_t* root, size_t dir_cluster);