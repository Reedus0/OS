#include "fat.h"
#include "bpb.h"
#include "fs/vfs.h"
#include "include/kalloc.h"
#include "include/fs.h"
#include "include/dev.h"
#include "include/macro.h"

struct fat_info* get_fat_info(dev_t* dev) {
    struct fat_bpb* bpb = get_bpb(dev);
    struct fat_info* fat_info = kalloc(sizeof(struct fat_info));

    fat_info->sectors_per_claster = bpb->sec_per_clus;
    fat_info->sector_size = bpb->byts_per_sec;
    fat_info->total_fats = bpb->num_fats;

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
    bdev_get_block(dev, fat_info->fats, fat_info->fat_region, fat_info->total_fats * fat_info->fat_size);

    switch (fat_info->fat_type) {
        case FAT12:
        case FAT16:
            fat_info->root_dir_region = fat_info->data_region - fat_info->total_root_dir_sectors;
            break;
        case FAT32:
            break;
    }

    kfree(bpb);

    return fat_info;
}

fat_entry_t* fat_get_cluster(dev_t* dev, struct fat_info* fat_info, size_t cluster) {
    fat_entry_t* fat_entry = kalloc(fat_info->sector_size * fat_info->sectors_per_claster);

    size_t entry_sector = ((cluster - 2) * fat_info->sectors_per_claster) + fat_info->data_region;

    bdev_get_block(dev, fat_entry, entry_sector, fat_info->sectors_per_claster);

    return fat_entry;
}

fat_data_t* fat_get_data(dev_t* dev, struct fat_info* fat_info, fat_entry_t* fat_entry) {
    fat_data_t* file_data = NULL;

    size_t cluster = fat_entry->cluster_low | fat_entry->cluster_high << 16;
    size_t fat_cluster = cluster;

    while ((fat_cluster & 0xFFF) != 0xFFF) {
        
        fat_data_t* file_data_chunk = kalloc(sizeof(fat_data_t));
        file_data_chunk->entry = fat_get_cluster(dev, fat_info, fat_cluster);

        file_data_chunk->list.next = NULL;
        file_data_chunk->list.prev = NULL;
        
        if (file_data != NULL) {
            list_insert_after(&file_data->list, &file_data_chunk->list);
        }
        file_data = file_data_chunk;

        switch (fat_info->fat_type) {
            case FAT12:
                uint16_t fat_index = cluster + (cluster / 2);
                if (fat_cluster % 2 == 0) {
                    fat_cluster = fat_info->fats[fat_index] << 4 | fat_info->fats[fat_index + 1] & 0x0F;
                } else {
                    fat_cluster = (fat_info->fats[fat_index] & 0xF0) >> 4 | (fat_info->fats[fat_index + 1] & 0xFF) << 4;
                }
                break;
            case FAT16:
                break;
            case FAT32:
                break;
        }
    }   
    return file_data;
}

void fat_add_file(dev_t* dev, struct fat_info* fat_info, fat_file_t* file_entry, dir_t* root) {
    file_t* new_file = vfs_new_file(file_entry->name);
    vfs_add_file(root, new_file);
}

void fat_add_dir(dev_t* dev, struct fat_info* fat_info, fat_dir_t* dir_entry, dir_t* root) {
    dir_t* new_dir = vfs_new_dir(dir_entry->name);
    vfs_add_subdir(root, new_dir);

    fat_data_t* dir = fat_get_data(dev, fat_info, dir_entry);

    while (1) {
        fat_parse_dir(dev, fat_info, dir->entry, new_dir);

        list_t* prev = dir->list.prev;
        kfree(dir);

        if (dir->list.prev == NULL) {
            break;
        }
        dir = container_of(prev, fat_data_t, list);
    }
}

void fat_parse_dir(dev_t* dev, struct fat_info* fat_info, fat_dir_t* dir, dir_t* root) {
    fat_entry_t* current_entry = dir;
    while (current_entry->name[0] != 0) {
        if (current_entry->attributes == LFN) {
            current_entry++;
            continue;
        }
        if (strncmp(current_entry->name, ".", 1) || strncmp(current_entry->name, "..", 2)) {
            current_entry++;
            continue;
        }
        if (current_entry->attributes == DIRECTORY) {
            fat_add_dir(dev, fat_info, current_entry, root);
        }
        if (current_entry->attributes == ARCHIVE) {
            fat_add_file(dev, fat_info, current_entry, root);
        }
        current_entry++;
    }
    kfree(dir);
}


void init(fs_t* fs, dev_t* dev, dir_t* root) {
    struct fat_info* fat_info = get_fat_info(dev);

    fat_dir_t* root_dir = kalloc(fat_info->total_root_dir_sectors * fat_info->sector_size);

    bdev_get_block(dev, root_dir, fat_info->root_dir_region, fat_info->total_root_dir_sectors);

    fat_parse_dir(dev, fat_info, root_dir, root);
    
    fs = fat_info;
}

void deinit(fs_t* fs, dev_t* dev, dir_t* root) {
    struct fat_info* fat_info = fs->fs_data;
    kfree(fat_info->fats);
    kfree(fat_info);
}

void write_file(fs_t* fs, dev_t* dev, char* path, byte* buffer, size_t count) {

}

void read_file(fs_t* fs, dev_t* dev, char* path, byte* buffer, size_t count) {
    
}

void create_file(fs_t* fs, dev_t* dev, char* path) {
    
}

void delete_file(fs_t* fs, dev_t* dev, char* path) {
    
}

void create_dir(fs_t* fs, dev_t* dev, char* path) {
    
}

void delete_dir(fs_t* fs, dev_t* dev, char* path) {
    
}