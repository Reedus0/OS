#pragma once

#include <stdint.h>
#include <stddef.h>

#include "fs.h"
#include "init.h"
#include "include/dev.h"

struct fat_cluster {
    byte* cluster;
    list_t list;
};
typedef struct fat_cluster fat_cluster_t;
typedef fat_cluster_t fat_vfs_file_t;

size_t fat_read_content(vfs_fs_t* fs, size_t index, byte buffer[], size_t offset, size_t count);
size_t fat_write_content(vfs_fs_t* fs, size_t index, byte buffer[], size_t offset, size_t count);
size_t fat_cluster_count(vfs_fs_t* fs, size_t index);
size_t fat_read_root(vfs_fs_t* fs, byte buffer[], size_t offset, size_t count);
size_t fat_write_root(vfs_fs_t* fs, byte buffer[], size_t offset, size_t count);