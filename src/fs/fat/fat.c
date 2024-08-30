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

vfs_fs_t* create_fs_fat(dev_t* dev) {
    vfs_fs_t* fs_fat = kalloc(sizeof(vfs_fs_t));

    fs_fat->dev = dev;
    fs_fat->func = &g_vfs_func_fat;

    return fs_fat;
}

void delete_fs_fat(vfs_fs_t* fs_fat) {
    kfree(fs_fat);
}

void init(vfs_fs_t* fs, vfs_dir_t* root) {
    fs->fs_data = get_fat_info(fs);

    fat_entry_t* root_dir = fat_read_root(fs, root);

    fat_parse_dir(fs, root_dir, root, 0);
}

void deinit(vfs_fs_t* fs, vfs_dir_t* root) {
    struct fat_info* fat_info = fs->fs_data;
    kfree(fat_info->fats);
    kfree(fat_info);
}

void read_file(vfs_fs_t* fs, vfs_file_t* file, byte* buffer, size_t count) {
    struct fat_info* fat_info = fs->fs_data;
    fat_file_data_t* file_data = file->fs_data;

    fat_cluster_t* file_content = fat_read_cluster(fs, file_data->cluster);

    size_t position = file->position;
    size_t current_block = 0;
    size_t read_bytes = 0;

    size_t index = file_data->cluster;

    while (1) {
        if (current_block >= position / fat_info->cluster_size) {
            read_bytes = fat_read(fs, file_content, buffer, position, count - read_bytes);
        }

        kfree(file_content->cluster);
        kfree(file_content);

        index = fat_read_table(fs, index);
        if ((index & fat_info->eof) == fat_info->eof) break;

        file_content = fat_read_cluster(fs, index);
        current_block++;
    }
}

void write_file(vfs_fs_t* fs, vfs_file_t* file, byte* buffer, size_t count) {
    struct fat_info* fat_info = fs->fs_data;
    fat_file_data_t* file_data = file->fs_data;

    fat_cluster_t* file_content = fat_read_cluster(fs, file_data->cluster);

    size_t position = file->position;
    size_t current_block = 0;
    size_t wrote_bytes = 0;

    size_t index = file_data->cluster;

    while (1) {
        if (current_block >= position / fat_info->cluster_size) {
            wrote_bytes = fat_write(fs, file_content, buffer, position, count - wrote_bytes);
        }

        fat_write_cluster(fs, index, file_content);

        if ((fat_read_table(fs, index) & fat_info->eof) == fat_info->eof) {
            if (wrote_bytes < count) {
                fat_table_allocate_cluster(fs, index);
            } else {
                break;
            }
        }

        index = fat_read_table(fs, index);
        file_content = fat_read_cluster(fs, index);
        current_block++;
    }
}

vfs_file_t* create_file(vfs_fs_t* fs, vfs_dir_t* dir, char* name) {
    
}

void delete_file(vfs_fs_t* fs, vfs_dir_t* dir, char* name) {

}

vfs_dir_t* create_dir(vfs_fs_t* fs, vfs_dir_t* parent, char* name) {
    
}

void delete_dir(vfs_fs_t* fs, vfs_dir_t* parent, char *name) {
    
}