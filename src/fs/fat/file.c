#include "file.h"

size_t fat_create_file(vfs_fs_t* fs, size_t dir_cluster, char* name, enum FAT_ATTRIBUTES attributes) {
    struct fat_info* fat_info = fs->fs_data;

    size_t content_size = 0;

    if (dir_cluster == ROOT_CLUSTER) {
        content_size = fat_info->total_root_dir_sectors * fat_info->sector_size;
    }
    else {
        size_t dir_size = fat_get_cluster_count(fs, dir_cluster);
        content_size = dir_size * fat_info->cluster_size;
    }

    fat_entry_t* buffer = kalloc(content_size);
    fat_read_content(fs, dir_cluster, buffer, 0, content_size);

    size_t free_cluster = fat_table_find_free_cluster(fs);
    fat_entry_t* new_entry = fat_entry_create(name, free_cluster, attributes);
    fat_write_table(fs, free_cluster, fat_info->eof | 0xF);

    fat_entry_t* ptr = new_entry;
    while (ptr->attributes == LFN) {
        fat_entry_add(buffer, ptr);
        ptr++;
    }
    fat_entry_add(buffer, ptr);
    kfree(new_entry);

    fat_write_content(fs, dir_cluster, buffer, 0, content_size);

    kfree(buffer);

    return free_cluster;
}

void fat_delete_file(vfs_fs_t* fs, size_t file_cluster, size_t dir_cluster, char* name) {
    struct fat_info* fat_info = fs->fs_data;

    size_t content_size = 0;

    if (dir_cluster == ROOT_CLUSTER) {
        content_size = fat_info->total_root_dir_sectors * fat_info->sector_size;
    }
    else {
        size_t dir_size = fat_get_cluster_count(fs, dir_cluster);
        content_size = dir_size * fat_info->cluster_size;
    }

    fat_entry_t* buffer = kalloc(content_size);
    fat_read_content(fs, dir_cluster, buffer, 0, content_size);

    size_t i = 0;
    fat_entry_t* ptr = buffer;
    char* entry_name = fat_entry_read_lfn(ptr);
    while (!strcmp(entry_name, name)) {
        ptr++;
        i++;
        kfree(entry_name);
        entry_name = fat_entry_read_lfn(ptr);
    }
    kfree(entry_name);

    while (ptr->attributes == LFN) fat_entry_remove(buffer, i);
    fat_entry_remove(buffer, i);

    fat_write_content(fs, dir_cluster, buffer, 0, content_size);

    kfree(buffer);

    while (1) {
        size_t next_cluster = fat_read_table(fs, file_cluster);
        fat_table_free_cluster(fs, file_cluster);
        if (next_cluster >= fat_info->eof) break;
        file_cluster = next_cluster;
    }
}