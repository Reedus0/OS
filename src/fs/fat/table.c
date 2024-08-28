#include "table.h"
#include "init.h"
#include "include/fs.h"

size_t fat_read_table(fs_t* fs, size_t index) {
    struct fat_info* fat_info = fs->fs_data;
    switch (fat_info->fat_type) {
        case FAT12:
            uint16_t fat_index = index + (index / 2);
            if (index % 2 == 0) {
                return (fat_info->fats[fat_index] & 0xFF) << 4 | (fat_info->fats[fat_index + 1] & 0xF);
            } else {
                return (fat_info->fats[fat_index] & 0xF0) << 4 | (fat_info->fats[fat_index + 1] & 0xFF);
            }
            break;
        case FAT16:
            break;
        case FAT32:
            break;
    }
}

void fat_write_table(fs_t* fs, size_t index, size_t data) {
    struct fat_info* fat_info = fs->fs_data;
    switch (fat_info->fat_type) {
        case FAT12:
            uint16_t fat_index = index + (index / 2);
            data = data & 0xFFF;
            if (index % 2 == 0) {
                fat_info->fats[fat_index] = (data & 0xFF0) >> 4;
                fat_info->fats[fat_index + 1] = (data & 0xF) | (fat_info->fats[fat_index + 1] & 0xF0);
            } else {
                fat_info->fats[fat_index] = ((data & 0xF00) >> 4) | (fat_info->fats[fat_index] & 0xF);
                fat_info->fats[fat_index + 1] = (data & 0xFF);
            }
            break;
        case FAT16:
            break;
        case FAT32:
            break;
    }
}

size_t fat_table_allocate_cluster(fs_t* fs, size_t cluster) {
    struct fat_info* fat_info = fs->fs_data;

    size_t result = 0;
    size_t fat_size = 0;
    
    switch (fat_info->fat_type) {
    case FAT12:
        fat_size = (fat_info->fat_size * fat_info->sector_size * 2) / 3;
        break;
    case FAT16:
        break;
    case FAT32:
        break;
    }

    for (size_t i = FIRST_DATA_CLUSTER; i < fat_size; i++) {
        size_t new_cluster = fat_read_table(fs, i);
        if ((new_cluster & fat_info->eof) == 0) {
            fat_write_table(fs, cluster, i);
            fat_write_table(fs, i, fat_info->eof);
            fat_commit_table(fs);
            printk("new: %x %x\n", cluster, i);
            return i;
        }
    }
}

void fat_table_free_cluster(fs_t* fs, size_t cluster) {
    fat_write_table(fs, cluster, 0);
    fat_commit_table(fs);
}

void fat_commit_table(fs_t* fs) {
    struct fat_info* fat_info = fs->fs_data;
    bdev_write_block(fs->dev, fat_info->fats, fat_info->fat_region, fat_info->total_fats * fat_info->fat_size);
}

