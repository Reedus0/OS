#include "fs.h"
#include "bpb.h"
#include "table.h"
#include "init.h"
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

    fat_read_root(fs, root_dir, 0, root_size);

    fat_parse_dir(fs, root_dir, fs->mount_point, ROOT_CLUSTER);

    kfree(root_dir);
}

void deinit(vfs_fs_t* fs) {
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

    fat_entry_t* buffer = NULL;

    if (dir_data->cluster == ROOT_CLUSTER) {
        size_t root_size = fat_info->total_root_dir_sectors * fat_info->sector_size;
        buffer = kalloc(root_size);
        fat_read_root(fs, buffer, 0, root_size);
    } else {
        size_t dir_size = fat_cluster_count(fs, dir_data->cluster);
        size_t content_size = dir_size * fat_info->cluster_size;
        buffer = kalloc(content_size);
        fat_read_content(fs, dir_data->cluster, buffer, 0, content_size);
    }

    size_t free_cluster = fat_table_find_free_cluster(fs); 
    fat_entry_t* new_entry = fat_entry_create(name, free_cluster, ARCHIVE);
    //fat_write_table(fs, free_cluster, fat_info->eof);

    fat_entry_t* ptr = new_entry;
    while (ptr->attributes == LFN) {
        fat_entry_add(buffer, ptr);
        ptr++;
    }
    fat_entry_add(buffer, ptr);
    kfree(new_entry);

    if (dir_data->cluster == ROOT_CLUSTER) {
        size_t root_size = fat_info->total_root_dir_sectors * fat_info->sector_size;
        fat_write_root(fs, buffer, 0, root_size);
    } else {
        size_t dir_size = fat_cluster_count(fs, dir_data->cluster);
        size_t content_size = dir_size * fat_info->cluster_size;
        fat_write_content(fs, dir_data->cluster, buffer, 0, content_size);
    }

    kfree(buffer);  

    fat_file_data_t* new_file_data = kalloc(sizeof(fat_file_data_t));

    new_file_data->cluster = free_cluster;
    new_file_data->dir_cluster = dir_data->cluster;

    return vfs_new_file(name, new_file_data);
}

void delete_file(vfs_fs_t* fs, vfs_file_t* file) {
    struct fat_info* fat_info = fs->fs_data;
    fat_file_data_t* file_data = file->fs_data;

    fat_entry_t* buffer = NULL;

    if (file_data->dir_cluster == ROOT_CLUSTER) {
        size_t root_size = fat_info->total_root_dir_sectors * fat_info->sector_size;
        buffer = kalloc(root_size);
        fat_read_root(fs, buffer, 0, root_size);
    } else {
        size_t dir_size = fat_cluster_count(fs, file_data->dir_cluster);
        size_t content_size = dir_size * fat_info->cluster_size;
        buffer = kalloc(content_size);
        fat_read_content(fs, file_data->dir_cluster, buffer, 0, content_size);
    }

    size_t i = 0;
    fat_entry_t* ptr = buffer;
    while (!strcmp(fat_entry_read_lfn(ptr), file->name)) {
        ptr++;
        i++;
    }
    while (ptr->attributes == LFN) fat_entry_remove(buffer, i);
    fat_entry_remove(buffer, i);

    if (file_data->dir_cluster == ROOT_CLUSTER) {
        size_t root_size = fat_info->total_root_dir_sectors * fat_info->sector_size;
        fat_write_root(fs, buffer, 0, root_size);
    } else {
        size_t dir_size = fat_cluster_count(fs, file_data->dir_cluster);
        size_t content_size = dir_size * fat_info->cluster_size;
        fat_write_content(fs, file_data->dir_cluster, buffer, 0, content_size);
    }

    kfree(buffer);
}

vfs_dir_t* create_dir(vfs_fs_t* fs, vfs_dir_t* parent, char* name) {
    
}

void delete_dir(vfs_fs_t* fs, vfs_dir_t* dir) {
    
}