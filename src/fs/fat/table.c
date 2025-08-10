#include "table.h"
#include "init.h"
#include "include/fs.h"

static size_t get_fat_size(vfs_fs_t* fs) {
    struct fat_info* fat_info = fs->fs_data;

    size_t fat_size = fat_info->fat_size;

    switch (fat_info->fat_type) {
    case FAT12:
        fat_size = (fat_info->fat_size * fat_info->sector_size * 2) / 3 + 1;
        break;
    case FAT16:
    case FAT32:
        fat_size = fat_info->fat_size * fat_info->sector_size * 2;
        break;
    }

    return fat_size;
}

static byte* read_table(vfs_fs_t* fs, size_t index) {
    struct fat_info* fat_info = fs->fs_data;

    size_t fat_size = get_fat_size(fs);

    byte* table = kalloc(fat_size);

    bdev_read(fs->dev, table, fat_info->fat_region, fat_info->fat_size);

    return table;
}

size_t fat_read_table(vfs_fs_t* fs, size_t index) {
    struct fat_info* fat_info = fs->fs_data;

    size_t result = 0;
    if (!fat_info->fat) fat_info->fat = read_table(fs, index);

    switch (fat_info->fat_type) {
    case FAT12:
        size_t fat_index = index + (index / 2);
        unsigned short short_value = *(unsigned short*)&fat_info->fat[fat_index];
        short_value = (index & 1) ? short_value >> 4 : short_value & 0xFFF;
        result = short_value;
        break;
    case FAT16:
        result = *(uint16_t*)&fat_info->fat[index * 2];
        break;
    case FAT32:
        break;
    }

    // printk(NONE, "%x->%x %x ", index, result, index * 2);

    return result;
}

static void write_table(vfs_fs_t* fs, size_t index, byte* table) {
    struct fat_info* fat_info = fs->fs_data;

    size_t fat_size = get_fat_size(fs);

    // for (size_t i = 0; i < fat_info->total_fats; i++) {
    //     size_t next_fat = fat_info->fat_region + fat_offset + (fat_info->fat_size * i);
    //     bdev_write(fs->dev, table, next_fat, 1);
    // }
    bdev_write(fs->dev, table, fat_info->fat_region, fat_size);

    return table;
}

void fat_write_table(vfs_fs_t* fs, size_t index, size_t data) {
    struct fat_info* fat_info = fs->fs_data;

    size_t result = 0;
    if (!fat_info->fat) fat_info->fat = read_table(fs, index);

    switch (fat_info->fat_type) {
    case FAT12:
        size_t fat_index = index + (index / 2);
        unsigned short* short_value = (unsigned short*)&fat_info->fat[fat_index];
        if (index % 2 == 0) {
            *short_value = (*short_value & 0xF000) | data & 0x0FFF;
        }
        else {
            *short_value = (*short_value & 0x0FFF) | ((data & 0x0FFF) << 4);
        }
        break;
    case FAT16:
        break;
    case FAT32:
        break;
    }

    write_table(fs, index, fat_info->fat);
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