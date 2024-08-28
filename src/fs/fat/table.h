#pragma once

#include <stdint.h>
#include <stddef.h>

#include "fat.h"
#include "init.h"
#include "include/dev.h"

size_t fat_read_table(fs_t* fs, size_t index);
void fat_write_table(fs_t* fs, size_t index, size_t data);
size_t fat_table_allocate_cluster(fs_t* fs, size_t cluster);
void fat_table_free_cluster(fs_t* fs, size_t cluster);
void fat_commit_table(fs_t* fs);
