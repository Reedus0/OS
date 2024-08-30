#include "file.h"
#include "init.h"
#include "fat.h"
#include "include/macro.h"
#include "include/types.h"

fat_cluster_t* fat_read_cluster(vfs_fs_t* fs, size_t index) {
    struct fat_info* fat_info = fs->fs_data;

    fat_cluster_t* file_cluster = kalloc(sizeof(fat_cluster_t));
    void* fat_entry = kalloc(fat_info->cluster_size);

    file_cluster->cluster = fat_entry;

    file_cluster->list.next = NULL;
    file_cluster->list.prev = NULL;    
    
    size_t entry_sector = ((index - 2) * fat_info->sectors_per_claster) + fat_info->data_region;

    bdev_read(fs->dev, file_cluster->cluster, entry_sector, fat_info->sectors_per_claster);
    
    return file_cluster;
}

size_t fat_read(vfs_fs_t* fs, fat_cluster_t* fat_cluster, byte* buffer, size_t offset, size_t count) {
    struct fat_info* fat_info = fs->fs_data;
    
    size_t read_bytes = 0;

    for (size_t i = offset % fat_info->cluster_size; i < fat_info->cluster_size; i++) {
        if (read_bytes >= count) {
            break;
        }
        *(buffer + read_bytes) = *(byte*)(fat_cluster->cluster + offset % fat_info->cluster_size);
        read_bytes++;
        offset++;
    }
    return read_bytes;
}

void fat_write_cluster(vfs_fs_t* fs, size_t index, fat_cluster_t* data) {
    struct fat_info* fat_info = fs->fs_data;

    size_t entry_sector = ((index - 2) * fat_info->sectors_per_claster) + fat_info->data_region;

    bdev_write(fs->dev, data->cluster, entry_sector, fat_info->sectors_per_claster);

    kfree(data->cluster);
    kfree(data);
} 

size_t fat_write(vfs_fs_t* fs, fat_cluster_t* fat_cluster, byte* buffer, size_t offset, size_t count) {
    struct fat_info* fat_info = fs->fs_data;
    
    size_t wrote_bytes = 0;

    for (size_t i = offset % fat_info->cluster_size; i < fat_info->cluster_size; i++) {
        if (wrote_bytes >= count) {
            break;
        }
        *(byte*)(fat_cluster->cluster + offset % fat_info->cluster_size) = *(buffer + wrote_bytes);
        wrote_bytes++;
        offset++;
    }
    return wrote_bytes;
}