#include "file.h"
#include "init.h"
#include "fat.h"
#include "include/macro.h"
#include "include/types.h"

fat_cluster_t* fat_read_cluster(fs_t* fs, size_t cluster) {
    struct fat_info* fat_info = fs->fs_data;

    fat_cluster_t* file_cluster = kalloc(sizeof(fat_cluster_t));
    void* fat_entry = kalloc(fat_info->cluster_size);

    file_cluster->cluster = fat_entry;

    file_cluster->list.next = NULL;
    file_cluster->list.prev = NULL;    
    
    size_t entry_sector = ((cluster - 2) * fat_info->sectors_per_claster) + fat_info->data_region;

    bdev_read_block(fs->dev, file_cluster->cluster, entry_sector, fat_info->sectors_per_claster);
    
    return file_cluster;
}

void fat_write_cluster(fs_t* fs, size_t cluster, fat_cluster_t* data) {
    struct fat_info* fat_info = fs->fs_data;

    size_t entry_sector = ((cluster - 2) * fat_info->sectors_per_claster) + fat_info->data_region;

    bdev_write_block(fs->dev, data, entry_sector, fat_info->sectors_per_claster);

    kfree(data);
} 