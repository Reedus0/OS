#pragma once

#include <stdint.h>
#include <stddef.h>

#include "fat.h"
#include "init.h"
#include "include/dev.h"

struct fat_cluster {
    byte* cluster;
    list_t list;
};
typedef struct fat_cluster fat_cluster_t;
typedef fat_cluster_t fat_vfs_file_t;

fat_cluster_t* fat_read_cluster(vfs_fs_t* fs, size_t cluster);
size_t fat_read(vfs_fs_t* fs, fat_cluster_t* fat_cluster, byte* buffer, size_t offset, size_t count);
void fat_write_cluster(vfs_fs_t* fs, size_t cluster, fat_cluster_t* data);
size_t fat_write(vfs_fs_t* fs, fat_cluster_t* fat_cluster, byte* buffer, size_t offset, size_t count);