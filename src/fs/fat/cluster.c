#include "cluster.h"
#include "init.h"
#include "fs.h"
#include "table.h"
#include "include/macro.h"
#include "include/types.h"

static fat_cluster_t* read_cluster(vfs_fs_t* fs, size_t index) {
    struct fat_info* fat_info = fs->fs_data;

    fat_cluster_t* fat_cluster = kalloc(sizeof(fat_cluster_t));
    void* fat_entry = kalloc(fat_info->cluster_size);

    fat_cluster->cluster = fat_entry;

    size_t entry_sector = ((index - 2) * fat_info->sectors_per_claster) + fat_info->data_region;

    bdev_read(fs->dev, fat_cluster->cluster, entry_sector, fat_info->sectors_per_claster);

    return fat_cluster;
}

static size_t fat_read_cluster(vfs_fs_t* fs, size_t index, byte buffer[], size_t offset, size_t count) {
    struct fat_info* fat_info = fs->fs_data;

    size_t current_cluster = 0;
    size_t read_bytes = 0;
    while (1) {
        bool correct_cluster = current_cluster >= offset / fat_info->cluster_size;
        if (correct_cluster) {
            fat_cluster_t* cluster_content = read_cluster(fs, index);
            for (size_t i = offset % fat_info->cluster_size; i < fat_info->cluster_size; i++) {
                if (read_bytes >= count) {
                    kfree(cluster_content->cluster);
                    kfree(cluster_content);
                    return read_bytes;
                }
                buffer[read_bytes] = cluster_content->cluster[offset % fat_info->cluster_size];
                offset++;
                read_bytes++;
            }
            kfree(cluster_content->cluster);
            kfree(cluster_content);
        }

        index = fat_read_table(fs, index);
        if ((index & fat_info->eof) == fat_info->eof) break;

        current_cluster++;
    }

    return read_bytes;
}

static void write_cluster(vfs_fs_t* fs, size_t index, fat_cluster_t* data) {
    struct fat_info* fat_info = fs->fs_data;

    size_t entry_sector = ((index - 2) * fat_info->sectors_per_claster) + fat_info->data_region;

    bdev_write(fs->dev, data->cluster, entry_sector, fat_info->sectors_per_claster);

    kfree(data->cluster);
    kfree(data);
}

static size_t fat_write_cluster(vfs_fs_t* fs, size_t index, byte buffer[], size_t offset, size_t count) {
    struct fat_info* fat_info = fs->fs_data;

    size_t current_cluster = 0;
    size_t wrote_bytes = 0;

    while (1) {
        bool correct_cluster = current_cluster >= offset / fat_info->cluster_size;
        if (correct_cluster) {
            fat_cluster_t* cluster_content = read_cluster(fs, index);
            for (size_t i = offset % fat_info->cluster_size; i < fat_info->cluster_size; i++) {
                if (wrote_bytes >= count) {
                    break;
                }
                cluster_content->cluster[offset % fat_info->cluster_size] = buffer[wrote_bytes];
                offset++;
                wrote_bytes++;
            }
            write_cluster(fs, index, cluster_content);
        }

        if ((fat_read_table(fs, index) & fat_info->eof) == fat_info->eof) {
            if (wrote_bytes < count) {
                fat_table_allocate_cluster(fs, index);
            }
            else {
                break;
            }
        }

        index = fat_read_table(fs, index);
        current_cluster++;
    }
    return wrote_bytes;
}

static byte* read_root(vfs_fs_t* fs) {
    struct fat_info* fat_info = fs->fs_data;

    size_t root_size = fat_info->total_root_dir_sectors * fat_info->sector_size;
    byte* buffer = kalloc(root_size);

    bdev_read(fs->dev, buffer, fat_info->root_dir_region, fat_info->total_root_dir_sectors);

    return buffer;
}

static size_t fat_read_root(vfs_fs_t* fs, byte buffer[], size_t offset, size_t count) {
    struct fat_info* fat_info = fs->fs_data;

    size_t root_size = fat_info->total_root_dir_sectors * fat_info->sector_size;
    byte* root_dir = read_root(fs);
    size_t read_bytes = 0;

    for (size_t i = offset; i < root_size; i++) {
        if (read_bytes >= count) {
            break;
        }
        buffer[read_bytes] = root_dir[i];
        read_bytes++;
    }

    kfree(root_dir);

    return read_bytes;
}

static void write_root(vfs_fs_t* fs, byte* buffer) {
    struct fat_info* fat_info = fs->fs_data;

    bdev_write(fs->dev, buffer, fat_info->root_dir_region, fat_info->total_root_dir_sectors);

    kfree(buffer);
    return;
}

static size_t fat_write_root(vfs_fs_t* fs, byte buffer[], size_t offset, size_t count) {
    struct fat_info* fat_info = fs->fs_data;

    size_t root_size = fat_info->total_root_dir_sectors * fat_info->sector_size;
    byte* root_dir = read_root(fs);
    size_t wrote_bytes = 0;

    for (size_t i = offset; i < root_size; i++) {
        if (wrote_bytes >= count) {
            break;
        }
        root_dir[i] = buffer[wrote_bytes];
        wrote_bytes++;
    }

    write_root(fs, root_dir);

    return wrote_bytes;
}

size_t fat_read_content(vfs_fs_t* fs, size_t index, byte buffer[], size_t offset, size_t count) {
    if (index == ROOT_CLUSTER) {
        fat_read_root(fs, buffer, offset, count);
    }
    else {
        fat_read_cluster(fs, index, buffer, offset, count);
    }
}

size_t fat_write_content(vfs_fs_t* fs, size_t index, byte buffer[], size_t offset, size_t count) {
    if (index == ROOT_CLUSTER) {
        fat_write_root(fs, buffer, offset, count);
    }
    else {
        fat_write_cluster(fs, index, buffer, offset, count);
    }
}

size_t fat_get_cluster_count(vfs_fs_t* fs, size_t index) {
    struct fat_info* fat_info = fs->fs_data;

    size_t result = 1;
    while (1) {
        index = fat_read_table(fs, index);
        if (index >= fat_info->eof) break;
        result++;
    }

    return result;
}