#pragma once

#include <stdint.h>
#include <stddef.h>

#include "fs.h"
#include "entry.h"
#include "table.h"
#include "cluster.h"

size_t fat_create_file(vfs_fs_t* fs, size_t dir_cluster, char* name, enum FAT_ATTRIBUTES attributes);
void fat_delete_file(vfs_fs_t* fs, size_t file_cluster, size_t dir_cluster, char* name);