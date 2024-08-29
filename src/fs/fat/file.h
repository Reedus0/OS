#pragma once

#include <stdint.h>
#include <stddef.h>

#include "fat.h"
#include "init.h"
#include "include/dev.h"

struct fat_cluster {
    void* cluster;
    list_t list;
};
typedef struct fat_cluster fat_cluster_t;
typedef fat_cluster_t fat_vfs_file_t;

fat_cluster_t* fat_read_cluster(fs_t* fs, size_t cluster);
void fat_write_cluster(fs_t* fs, size_t cluster, fat_cluster_t* data);