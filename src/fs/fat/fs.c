#include "fs.h"
#include "bpb.h"
#include "table.h"
#include "init.h"
#include "file.h"
#include "cluster.h"
#include "fs/vfs.h"
#include "include/kalloc.h"
#include "include/fs.h"
#include "include/macro.h"

void init(vfs_fs_t* fs) {
    fs->fs_data = fat_get_info(fs);
    struct fat_info* fat_info = fs->fs_data;

    fat_file_data_t* root_data = kalloc(sizeof(fat_file_data_t));

    root_data->cluster = 0;
    root_data->dir_cluster = 0;

    fs->mount_point->fs_data = root_data;

    size_t root_size = fat_info->total_root_dir_sectors * fat_info->sector_size;
    byte* root_dir = kalloc(root_size);

    fat_read_content(fs, ROOT_CLUSTER, root_dir, 0, root_size);

    fat_parse_dir(fs, root_dir, fs->mount_point, ROOT_CLUSTER);

    kfree(root_dir);
}

void deinit(vfs_fs_t* fs) {
    struct fat_info* fat_info = fs->fs_data;

    kfree(fat_info->fat);
    kfree(fs->fs_data);
    kfree(fs->mount_point->fs_data);
}

size_t read_file(vfs_fs_t* fs, vfs_file_t* file, byte* buffer, size_t offset, size_t count) {
    struct fat_info* fat_info = fs->fs_data;
    fat_file_data_t* file_data = file->fs_data;

    fat_read_content(fs, file_data->cluster, buffer, offset, count);
}

size_t write_file(vfs_fs_t* fs, vfs_file_t* file, byte* buffer, size_t offset, size_t count) {
    struct fat_info* fat_info = fs->fs_data;
    fat_file_data_t* file_data = file->fs_data;

    fat_write_content(fs, file_data->cluster, buffer, offset, count);
}

vfs_file_t* create_file(vfs_fs_t* fs, vfs_dir_t* dir, char* name) {
    struct fat_info* fat_info = fs->fs_data;
    fat_file_data_t* dir_data = dir->fs_data;

    size_t new_file_cluster = fat_create_file(fs, dir_data->cluster, name, ARCHIVE);

    fat_file_data_t* new_file_data = kalloc(sizeof(fat_file_data_t));

    new_file_data->cluster = new_file_cluster;
    new_file_data->dir_cluster = dir_data->cluster;

    return vfs_new_file(name, new_file_data);
}

void delete_file(vfs_fs_t* fs, vfs_file_t* file) {
    struct fat_info* fat_info = fs->fs_data;
    fat_file_data_t* file_data = file->fs_data;

    fat_delete_file(fs, file_data->cluster, file_data->dir_cluster, file->name);
}

vfs_dir_t* create_dir(vfs_fs_t* fs, vfs_dir_t* dir, char* name) {
    struct fat_info* fat_info = fs->fs_data;
    fat_file_data_t* dir_data = dir->fs_data;

    size_t new_dir_cluster = fat_create_file(fs, dir_data->cluster, name, DIRECTORY);

    fat_file_data_t* new_dir_data = kalloc(sizeof(fat_file_data_t));

    new_dir_data->cluster = new_dir_cluster;
    new_dir_data->dir_cluster = dir_data->cluster;

    return vfs_new_dir(name, new_dir_data);
}

void delete_dir(vfs_fs_t* fs, vfs_dir_t* dir) {
    struct fat_info* fat_info = fs->fs_data;
    fat_file_data_t* dir_data = dir->fs_data;

    fat_delete_file(fs, dir_data->cluster, dir_data->dir_cluster, dir->name);
}