#include "fat.h"
#include "bpb.h"
#include "table.h"
#include "init.h"
#include "file.h"
#include "fs/vfs.h"
#include "include/kalloc.h"
#include "include/fs.h"
#include "include/dev.h"
#include "include/macro.h"

fs_t* create_fs_fat(dev_t* dev) {
    fs_t* fs_fat = kalloc(sizeof(fs_t));

    fs_fat->dev = dev;
    fs_fat->func = &g_fs_func_fat;

    return fs_fat;
}

void delete_fs_fat(fs_t* fs_fat) {
    //kfree(fs_fat);
}

void init(fs_t* fs, dir_t* root) {
    fs->fs_data = get_fat_info(fs);

    fat_parse_root(fs, root);
}

void deinit(fs_t* fs, dir_t* root) {
    struct fat_info* fat_info = fs->fs_data;
    //kfree(fat_info->fats);
    //kfree(fat_info);
}

void read_file(fs_t* fs, file_t* file, byte* buffer, size_t count) {
    struct fat_info* fat_info = fs->fs_data;
    fat_file_data_t* file_data = file->fs_data;

    fat_cluster_t* file_content = fat_read_cluster(fs, file_data->cluster);

    size_t position = file->position;
    size_t current_block = 0;
    size_t read_bytes = 0;

    size_t cluster = file_data->cluster;

    while (1) {
        if (current_block >= position / fat_info->cluster_size) {
            for (size_t i = position % fat_info->cluster_size; i < fat_info->cluster_size; i++) {
                *(buffer + read_bytes) = *(byte*)(file_content->cluster + position % fat_info->cluster_size);
                read_bytes++;
                position++;
                if (read_bytes >= count) {
                    break;
                }
            }
        }

        cluster = fat_read_table(fs, cluster);
        if ((cluster & fat_info->eof) == fat_info->eof) break;

        file_content = fat_read_cluster(fs, cluster);
        current_block++;
    }
}

void write_file(fs_t* fs, file_t* file, byte* buffer, size_t count) {
    struct fat_info* fat_info = fs->fs_data;
    fat_file_data_t* file_data = file->fs_data;

    fat_cluster_t* file_content = fat_read_cluster(fs, file_data->cluster);

    size_t position = file->position;
    size_t current_block = 0;
    size_t wrote_bytes = 0;

    size_t cluster = file_data->cluster;

    while (1) {
        if (current_block >= position / fat_info->cluster_size) {
            for (size_t i = position % fat_info->cluster_size; i < fat_info->cluster_size; i++) {
                if (wrote_bytes >= count) {
                    break;
                }
                *(byte*)(file_content->cluster + position % fat_info->cluster_size) = *(buffer + wrote_bytes);
                wrote_bytes++;
                position++;
            }
        }

        fat_write_cluster(fs, cluster, file_content->cluster);

        if ((fat_read_table(fs, cluster) & fat_info->eof) == fat_info->eof) {
            if (wrote_bytes < count) {
                fat_table_allocate_cluster(fs, cluster);
            } else {
                break;
            }
        }

        cluster = fat_read_table(fs, cluster);
        file_content = fat_read_cluster(fs, cluster);
        current_block++;
    }
}

file_t* create_file(fs_t* fs, dir_t* dir, char* name) {
    
}

void delete_file(fs_t* fs, dir_t* dir, char* name) {

}

dir_t* create_dir(fs_t* fs, dir_t* parent, char* name) {
    
}

void delete_dir(fs_t* fs, dir_t* parent, char *name) {
    
}