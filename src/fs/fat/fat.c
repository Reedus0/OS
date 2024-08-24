#include "fat.h"
#include "bpb.h"
#include "fs/vfs.h"
#include "include/kalloc.h"
#include "include/fs.h"
#include "include/dev.h"

byte* get_fats(dev_t* dev, struct fat_data* fat_data) {
    byte* fats = kalloc(fat_data->total_fats * fat_data->fat_size * fat_data->sector_size);

    bdev_get_block(dev, fats, fat_data->fat_region, fat_data->total_fats * fat_data->fat_size);

    return fats;
}

struct fat_data* get_fat_data(dev_t* dev) {
    struct fat_bpb* bpb = get_bpb(dev);
    struct fat_data* fat_data = kalloc(sizeof(struct fat_data));

    fat_data->sectors_per_claster = bpb->sec_per_clus;
    fat_data->sector_size = bpb->byts_per_sec;
    fat_data->total_fats = bpb->num_fats;

    fat_data->total_root_dir_sectors = ((bpb->root_ent_cnt * 32) + (bpb->byts_per_sec - 1)) / bpb->byts_per_sec;
    fat_data->fat_size = bpb->fat_sz_16 != 0 ? bpb->fat_sz_16 : bpb->fat_bpb_32.fat_sz_32;

    fat_data->data_region = bpb->rsvd_sec_cnt + (bpb->num_fats * fat_data->fat_size) + fat_data->total_root_dir_sectors;

    fat_data->total_sectors = bpb->tot_sec_16 != 0 ? bpb->tot_sec_16 : bpb->tot_sec_32;
    fat_data->fat_region = bpb->rsvd_sec_cnt;

    if (fat_data->total_sectors < 4085) {
        fat_data->fat_type = FAT12;
    } else if (fat_data->total_sectors < 65525) {
        fat_data->fat_type = FAT16;
    } else {
        fat_data->fat_type = FAT32;
    }

    fat_data->fats = get_fats(dev, fat_data);

    switch (fat_data->fat_type) {
        case FAT12:
        case FAT16:
            fat_data->root_dir_region = fat_data->data_region - fat_data->total_root_dir_sectors;
            break;
        case FAT32:
            break;
    }

    kfree(bpb);

    return fat_data;
}

size_t fat_get_entry_sector(struct fat_data* fat_data, uint16_t cluster) {
    return ((cluster - 2) * fat_data->sectors_per_claster) + fat_data->data_region;
}

file_t* fat_create_file(fat_entry_t* file_entry) {
    file_t* new_file = kalloc(sizeof(file_t));

    new_file->name = kalloc(12);
    strncpy(new_file->name, file_entry->name, 11);
    new_file->name[11] = 0;

    return new_file;
}

dir_t* fat_create_dir(fat_entry_t* dir_entry) {
    dir_t* new_dir = kalloc(sizeof(dir_t));

    new_dir->name = kalloc(12);
    strncpy(new_dir->name, dir_entry->name, 11);
    new_dir->name[11] = 0;

    new_dir->files.next = NULL;
    new_dir->files.prev = NULL;

    new_dir->subdirs.next = NULL;
    new_dir->subdirs.prev = NULL;

    return new_dir;
}

fat_entry_t* fat_get_cluster(dev_t* dev, struct fat_data* fat_data, size_t cluster) {
    fat_entry_t* fat_entry = kalloc(fat_data->sector_size * fat_data->sectors_per_claster);

    size_t entry_sector = fat_get_entry_sector(fat_data, cluster);

    bdev_get_block(dev, fat_entry, entry_sector, fat_data->sectors_per_claster);

    return fat_entry;
}

void fat_add_file(dev_t* dev, struct fat_data* fat_data, fat_entry_t* dir_entry, dir_t* root) {
    file_t* new_file = fat_create_file(dir_entry);
    vfs_add_file(root, new_file);
 
    size_t cluster = dir_entry->cluster_low | dir_entry->cluster_high << 16;
    size_t fat_cluster = cluster;
    while ((fat_cluster & 0xFFF) != 0xFFF) {
        uint16_t fat_index = cluster + (cluster / 2);
        if (fat_cluster % 2 == 0) {
            fat_cluster = fat_data->fats[fat_index] << 4 | fat_data->fats[fat_index + 1] & 0x0F;
        } else {
            fat_cluster = (fat_data->fats[fat_index] & 0xF0) >> 4 | (fat_data->fats[fat_index + 1] & 0xFF) << 4;
        }
    }
}

void fat_add_dir(dev_t* dev, struct fat_data* fat_data, fat_entry_t* dir_entry, dir_t* root) {
    dir_t* new_dir = fat_create_dir(dir_entry);
    vfs_add_subdir(root, new_dir);

    size_t cluster = dir_entry->cluster_low | dir_entry->cluster_high << 16;
    uint16_t fat_cluster = cluster;
    while ((fat_cluster & 0xFFF) != 0xFFF) {
        uint16_t fat_index = fat_cluster + (fat_cluster / 2);
        fat_entry_t* new_entry = fat_get_cluster(dev, fat_data, fat_cluster);
        fat_parse_dir(dev, fat_data, new_entry, new_dir);
        if (fat_cluster % 2 == 0) {
            fat_cluster = fat_data->fats[fat_index] << 4 | fat_data->fats[fat_index + 1] & 0x0F;
        } else {
            fat_cluster = (fat_data->fats[fat_index] & 0xF0) >> 4 | (fat_data->fats[fat_index + 1] & 0xFF) << 4;
        }
    }
}

void fat_parse_dir(dev_t* dev, struct fat_data* fat_data, fat_entry_t* fat_root, dir_t* root) {
    fat_entry_t* current_entry = fat_root;
    while (current_entry->name[0] != 0) {
        if (strncmp(current_entry->name, ".", 1) || strncmp(current_entry->name, "..", 2)) {
            current_entry++;
            continue;
        }
        if (current_entry->attributes == DIRECTORY) {
            fat_add_dir(dev, fat_data, current_entry, root);
        }
        if (current_entry->attributes == ARCHIVE) {
            fat_add_file(dev, fat_data, current_entry, root);
        }
        current_entry++;
    }
    kfree(fat_root);
}


void init(dev_t* dev, dir_t* root) {
    struct fat_data* fat_data = get_fat_data(dev);

    fat_entry_t* root_dir = kalloc(fat_data->total_root_dir_sectors * fat_data->sector_size);

    bdev_get_block(dev, root_dir, fat_data->root_dir_region, fat_data->total_root_dir_sectors);

    fat_parse_dir(dev, fat_data, root_dir, root);
    
    g_fs_fat.fs_data = fat_data;
}

void write_file(dev_t* dev, char* path, byte* buffer, size_t count) {

}

void read_file(dev_t* dev, char* path, byte* buffer, size_t count) {
    
}

void create_file(dev_t* dev, char* path) {
    
}

void delete_file(dev_t* dev, char* path) {
    
}

void create_dir(dev_t* dev, char* path) {
    
}

void delete_dir(dev_t* dev, char* path) {
    
}