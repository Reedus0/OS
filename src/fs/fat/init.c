#include "init.h"
#include "bpb.h"
#include "include/fs.h"
#include "include/macro.h"
#include "include/list.h"
#include "lib/string.h"

struct fat_info* get_fat_info(vfs_fs_t* fs) {
    struct fat_bpb* bpb = get_bpb(fs->dev);
    struct fat_info* fat_info = kalloc(sizeof(struct fat_info));

    fat_info->sectors_per_claster = bpb->sec_per_clus;
    fat_info->sector_size = bpb->byts_per_sec;
    fat_info->total_fats = bpb->num_fats;
    fat_info->cluster_size = fat_info->sector_size * fat_info->sectors_per_claster;

    fat_info->total_root_dir_sectors = ((bpb->root_ent_cnt * 32) + (bpb->byts_per_sec - 1)) / bpb->byts_per_sec;
    fat_info->fat_size = bpb->fat_sz_16 != 0 ? bpb->fat_sz_16 : bpb->fat_bpb_32.fat_sz_32;

    fat_info->data_region = bpb->rsvd_sec_cnt + (bpb->num_fats * fat_info->fat_size) + fat_info->total_root_dir_sectors;

    fat_info->total_sectors = bpb->tot_sec_16 != 0 ? bpb->tot_sec_16 : bpb->tot_sec_32;
    fat_info->fat_region = bpb->rsvd_sec_cnt;

    if (fat_info->total_sectors < 4085) {
        fat_info->fat_type = FAT12;
    } else if (fat_info->total_sectors < 65525) {
        fat_info->fat_type = FAT16;
    } else {
        fat_info->fat_type = FAT32;
    }

    fat_info->fats = kalloc(fat_info->total_fats * fat_info->fat_size * fat_info->sector_size);
    bdev_read(fs->dev, fat_info->fats, fat_info->fat_region, fat_info->total_fats * fat_info->fat_size);

    switch (fat_info->fat_type) {
        case FAT12:
            fat_info->eof = 0xFFF;
            fat_info->root_dir_region = fat_info->data_region - fat_info->total_root_dir_sectors;
            break;
        case FAT16:
            fat_info->eof = 0xFFFF;
            fat_info->root_dir_region = fat_info->data_region - fat_info->total_root_dir_sectors;
            break;
        case FAT32:
            fat_info->eof = 0xFFFFFFFF;
            break;
    }

    kfree(bpb);

    return fat_info;
}

vfs_file_t* fat_add_file(fat_entry_t* file_entry, vfs_dir_t* root, size_t dir_cluster, char* name);
vfs_dir_t* fat_add_dir(fat_entry_t* dir_entry, vfs_dir_t* root, size_t dir_cluster, char* name);

fat_entry_t* fat_parse_lfn(vfs_fs_t* fs, fat_entry_t* fat_entry, vfs_dir_t* root, size_t dir_cluster) {
    if (fat_entry->name[0] == 0xE5) {
        return fat_entry + 1;
    }
    char* name = fat_entry_get_lfn(fat_entry);

    while (fat_entry->attributes == LFN) fat_entry++;

    if (fat_entry->attributes == DIRECTORY) {
        vfs_dir_t* new_dir = fat_add_dir(fat_entry, root, dir_cluster, name);
        fat_parse_subdirs(fs, fat_entry, new_dir);
    }
    if (fat_entry->attributes == ARCHIVE) {
        fat_add_file(fat_entry, root, dir_cluster, name);
    }

    kfree(name);
    return fat_entry + 1;
}

vfs_file_t* fat_add_file(fat_entry_t* file_entry, vfs_dir_t* root, size_t dir_cluster, char* name) {
    fat_file_data_t* file_data = kalloc(sizeof(fat_file_data_t));

    file_data->cluster = file_entry->cluster_low | (uint32_t)file_entry->cluster_high << 16;
    file_data->dir_cluster = dir_cluster;

    vfs_file_t* new_file = vfs_new_file(name, file_data);
    vfs_add_file(root, new_file);

    return new_file;
}

vfs_dir_t* fat_add_dir(fat_entry_t* dir_entry, vfs_dir_t* root, size_t dir_cluster, char* name) {
    fat_file_data_t* dir_data = kalloc(sizeof(fat_file_data_t));

    dir_data->cluster = dir_entry->cluster_low | (uint32_t)dir_entry->cluster_high << 16;
    dir_data->dir_cluster = dir_cluster;

    vfs_dir_t* new_dir = vfs_new_dir(name, dir_data);
    vfs_add_subdir(root, new_dir);

    return new_dir;
}

void fat_parse_subdirs(vfs_fs_t* fs, fat_entry_t* dir_entry, vfs_dir_t* root) {
    struct fat_info* fat_info = fs->fs_data;

    size_t cluster = dir_entry->cluster_low | (uint32_t)dir_entry->cluster_high << 16;
    size_t fat_cluster = cluster;
    fat_cluster_t* dir = fat_read_cluster(fs, fat_cluster);

    while (1) {
        fat_parse_dir(fs, dir->cluster, root, cluster);
        kfree(dir);

        fat_cluster = fat_read_table(fs, fat_cluster);
        if ((fat_cluster & fat_info->eof) == fat_info->eof) break;

        dir = fat_read_cluster(fs, fat_cluster);
    }
}

void fat_parse_dir(vfs_fs_t* fs, fat_entry_t* dir, vfs_dir_t* root, size_t dir_cluster) {
    fat_entry_t* current_entry = dir;
    while (current_entry->name[0] != 0) {
        if (strncmp(current_entry->name, ".", 1) || strncmp(current_entry->name, "..", 2)) {
            current_entry++;
            continue;
        }
        if (current_entry->attributes == LFN) {
            current_entry = fat_parse_lfn(fs, current_entry, root, dir_cluster);
            continue;
        }
        if (current_entry->attributes == DIRECTORY) {
            vfs_dir_t* new_dir = fat_add_dir(current_entry, root, dir_cluster, current_entry->name);
            fat_parse_subdirs(fs, current_entry, new_dir);
        }
        if (current_entry->attributes == ARCHIVE) {
            fat_add_file(current_entry, root, dir_cluster, current_entry->name);
        }
        current_entry++;
    }
    kfree(dir);
}

fat_entry_t* fat_read_root(vfs_fs_t* fs) {
    struct fat_info* fat_info = fs->fs_data;

    fat_entry_t* root_dir = kalloc(fat_info->total_root_dir_sectors * fat_info->sector_size);
    bdev_read(fs->dev, root_dir, fat_info->root_dir_region, fat_info->total_root_dir_sectors);

    return root_dir;
}