#include "table.h"
#include "init.h"
#include "include/fs.h"

static size_t get_fat_size(vfs_fs_t* fs) {
    struct fat_info* fat_info = fs->fs_data;

    size_t fat_size = 0;

    switch (fat_info->fat_type) {
        case FAT12:
            fat_size = (fat_info->fat_size * fat_info->sector_size * 2) / 3 + 1;
            break;
        case FAT16:
            break;
        case FAT32:
            break;
    }

    return fat_size;
}

static byte* read_table(vfs_fs_t* fs, size_t index) {
    struct fat_info* fat_info = fs->fs_data;

    size_t fat_size = get_fat_size(fs);
    size_t fat_offset = index / fat_size;

    byte* table = kalloc(fat_info->sector_size);
    bdev_read(fs->dev, table, fat_info->fat_region + fat_offset, 1);

    return table;
}

size_t fat_read_table(vfs_fs_t* fs, size_t index) {
    struct fat_info* fat_info = fs->fs_data;

    size_t result = 0;
    byte* table = read_table(fs, index);

    switch (fat_info->fat_type) {
        case FAT12:
            size_t fat_index = index + (index / 2);
            if (index % 2 == 0) {
                result = (table[fat_index] & 0xFF) << 4 | (table[fat_index + 1] & 0xF);
            } else {
                result = (table[fat_index] & 0xF0) << 4 | (table[fat_index + 1] & 0xFF);
            }
            break;
        case FAT16:
            break;
        case FAT32:
            break;
    }

    kfree(table);
    return result;
}

static void write_table(vfs_fs_t* fs, size_t index, byte* table) {
    struct fat_info* fat_info = fs->fs_data;

    size_t fat_size = get_fat_size(fs);
    size_t fat_offset = index / fat_size;

    // for (size_t i = 0; i < fat_info->total_fats; i++) {
    //     size_t next_fat = fat_info->fat_region + fat_offset + (fat_info->fat_size * i);
    //     bdev_write(fs->dev, table, next_fat, 1);
    // }
    bdev_write(fs->dev, table, fat_info->fat_region + fat_offset, 1);

    return table;
}

void fat_write_table(vfs_fs_t* fs, size_t index, size_t data) {
    struct fat_info* fat_info = fs->fs_data;

    size_t result = 0;
    byte* table = read_table(fs, index);

    switch (fat_info->fat_type) {
        case FAT12:
            size_t fat_index = index + (index / 2);
            if (index % 2 == 0) {
                table[fat_index] = (data & 0xFF0) >> 4;
                table[fat_index + 1] = (data & 0xF) | (table[fat_index + 1] & 0xF0);
            } else {
                table[fat_index] = ((data & 0xF00) >> 4) | (table[fat_index] & 0xF);
                table[fat_index + 1] = (data & 0xFF);
            }
            break;
        case FAT16:
            break;
        case FAT32:
            break;
    }

    write_table(fs, index, table);
    kfree(table);
}

size_t fat_table_find_free_cluster(vfs_fs_t* fs) {
    struct fat_info* fat_info = fs->fs_data;

    size_t fat_size = get_fat_size(fs);
    for (size_t i = FIRST_DATA_CLUSTER; i < fat_size; i++) {
        size_t new_cluster = fat_read_table(fs, i);
        if ((new_cluster & fat_info->eof) == 0) {
            return i;
        }
    }
}

size_t fat_table_allocate_cluster(vfs_fs_t* fs, size_t cluster) {
    struct fat_info* fat_info = fs->fs_data;

    size_t fat_size = get_fat_size(fs);
    size_t free_cluster = fat_table_find_free_cluster(fs);

    fat_write_table(fs, cluster, free_cluster);
    fat_write_table(fs, free_cluster, fat_info->eof);
}


void fat_table_free_cluster(vfs_fs_t* fs, size_t cluster) {
    fat_write_table(fs, cluster, 0);
}