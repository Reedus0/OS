#pragma once

#include <stdint.h>
#include <stddef.h>

// Reference: https://www.cs.fsu.edu/~cop4610t/assignments/project3/spec/fatspec.pdf
// Reference: https://wiki.osdev.org/FAT

#include "include/fs.h"

#define FIRST_DATA_CLUSTER 5
#define ROOT_CLUSTER 0

struct fat_file_data {
    uint32_t cluster;
    uint32_t dir_cluster;
};
typedef struct fat_file_data fat_file_data_t;

vfs_func_t* init_vfs_func_fat();