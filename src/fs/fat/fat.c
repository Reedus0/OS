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
    bdev_read_block(dev, fat_info->fats, fat_info->fat_region, fat_info->total_fats * fat_info->fat_size);

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

size_t fat_read_table(struct fat_info* fat_info, size_t index) {
    switch (fat_info->fat_type) {
        case FAT12:
            uint16_t fat_index = index + (index / 2);
            if (index % 2 == 0) {
                return fat_info->fats[fat_index] << 4 | fat_info->fats[fat_index + 1] & 0x0F;
            } else {
                return (fat_info->fats[fat_index] & 0xF0) >> 4 | (fat_info->fats[fat_index + 1] & 0xFF) << 4;
            }
            break;
        case FAT16:
            break;
        case FAT32:
            break;
    }
}

void fat_write_table(struct fat_info* fat_info, size_t index, size_t data) {
    switch (fat_info->fat_type) {
        case FAT12:
            uint16_t fat_index = index + (index / 2);
            data = data & 0xFFF;
            if (index % 2 == 0) {
                fat_info->fats[fat_index] = (data & 0xFF0) >> 4;
                fat_info->fats[fat_index + 1] = data & 0x0F;
            } else {
                fat_info->fats[fat_index] = (data & 0xF) << 4;
                fat_info->fats[fat_index + 1] = (data & 0xFF0) >> 4;
            }
            break;
        case FAT16:
            break;
        case FAT32:
            break;
    }
}

void fat_commit_table(dev_t* dev, struct fat_info* fat_info) {
    bdev_write_block(dev, fat_info->fats, fat_info->fat_region, fat_info->total_fats * fat_info->fat_size);
}

fat_entry_t* fat_read_cluster(dev_t* dev, struct fat_info* fat_info, size_t cluster) {
    fat_entry_t* fat_entry = kalloc(fat_info->cluster_size);

    size_t entry_sector = ((cluster - 2) * fat_info->sectors_per_claster) + fat_info->data_region;

    bdev_read_block(dev, fat_entry, entry_sector, fat_info->sectors_per_claster);

    return fat_entry;
}

fat_data_t* fat_read_data(dev_t* dev, struct fat_info* fat_info, size_t cluster) {
    fat_data_t* file_data = NULL;

    size_t fat_cluster = cluster;

    while ((fat_cluster & fat_info->eof) != fat_info->eof) {

        fat_data_t* file_data_chunk = kalloc(sizeof(fat_data_t));
        
        file_data_chunk->entry = fat_read_cluster(dev, fat_info, fat_cluster);

        file_data_chunk->list.next = NULL;
        file_data_chunk->list.prev = NULL;
        
        if (file_data != NULL) {
            list_insert_after(&file_data->list, &file_data_chunk->list);
        }
        file_data = file_data_chunk;

        fat_cluster = fat_read_table(fat_info, fat_cluster);
    }   
    return file_data;
}

void fat_write_cluster(dev_t* dev, struct fat_info* fat_info, size_t cluster, void* data) {
    size_t entry_sector = ((cluster - 2) * fat_info->sectors_per_claster) + fat_info->data_region;

    bdev_write_block(dev, data, entry_sector, fat_info->sectors_per_claster);
}

void fat_write_data(dev_t* dev, struct fat_info* fat_info, size_t cluster, fat_data_t* fat_data) {
    size_t fat_cluster = cluster;

    while (1) {

        fat_write_cluster(dev, fat_info, fat_cluster, fat_data->entry);

        list_t* prev = fat_data->list.prev;
        kfree(fat_data);

        if (prev == NULL) {
            break;
        }

        if (fat_cluster == fat_info->eof) {
            size_t fat_size = 0;
            switch (fat_info->fat_type) {
            case FAT12:
                fat_size = (fat_info->fat_size * fat_info->sector_size * 2) / 3;
                break;
            case FAT16:
                break;
            case FAT32:
                break;
            }
            for (size_t i = 0; i < fat_size; i++) {
                size_t new_cluster = fat_read_table(fat_info, i);
                if ((new_cluster & fat_info->eof) == 0) {
                    fat_cluster = i;
                    fat_write_table(fat_info, i, fat_info->eof);
                    break;
                };
            }
        }

        fat_cluster = fat_read_table(fat_info, fat_cluster);
        fat_data = container_of(prev, fat_data_t, list);
    }  
    fat_commit_table(dev, fat_info);
}

void fat_add_file(dev_t* dev, struct fat_info* fat_info, fat_file_t* file_entry, dir_t* root) {
    fat_file_data_t* file_data = kalloc(sizeof(fat_file_data_t));

    file_data->cluster = file_entry->cluster_low | file_entry->cluster_high << 16;
    file_t* new_file = vfs_new_file(file_entry->name, file_data);
    vfs_add_file(root, new_file);
}

void fat_add_dir(dev_t* dev, struct fat_info* fat_info, fat_dir_t* dir_entry, dir_t* root) {
    fat_file_data_t* dir_data = kalloc(sizeof(fat_file_data_t));

    size_t cluster = dir_entry->cluster_low | dir_entry->cluster_high << 16;
    fat_data_t* dir = fat_read_data(dev, fat_info, cluster);

    dir_data->cluster = dir_entry->cluster_low | dir_entry->cluster_high << 16;
    dir_t* new_dir = vfs_new_dir(dir_entry->name, dir_data);
    vfs_add_subdir(root, new_dir);

    while (1) {
        fat_parse_dir(dev, fat_info, dir->entry, new_dir);

        list_t* prev = dir->list.prev;
        kfree(dir);

        if (prev == NULL) {
            break;
        }
        dir = container_of(prev, fat_data_t, list);
    }
}

void fat_parse_dir(dev_t* dev, struct fat_info* fat_info, fat_dir_t* dir, dir_t* root) {
    fat_entry_t* current_entry = dir;
    while (current_entry->name[0] != 0) {
        if (current_entry->attributes & LFN == LFN) {
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

    bdev_read_block(dev, root_dir, fat_info->root_dir_region, fat_info->total_root_dir_sectors);

    fat_parse_dir(dev, fat_info, root_dir, root);
    
    fs->fs_data = fat_info;
}

void deinit(fs_t* fs, dev_t* dev, dir_t* root) {
    struct fat_info* fat_info = fs->fs_data;
    kfree(fat_info->fats);
    kfree(fat_info);
}

void read_file(fs_t* fs, dev_t* dev, file_t* file, byte* buffer, size_t count) {
    struct fat_info* fat_info = fs->fs_data;
    fat_file_data_t* file_data = file->fs_data;

    fat_data_t* file_content = fat_read_data(dev, fat_info, file_data->cluster);

    size_t position = file->position;
    size_t read_bytes = 0;
    
    while (1) {
        for (size_t i = 0; i < fat_info->cluster_size; i++) {
            *(buffer + read_bytes) = *(byte*)(file_content->entry + position % fat_info->cluster_size);
            read_bytes++;
            position++;
            if (read_bytes >= count) {
                break;
            }
        }

        list_t* prev = file_content->list.prev;
        kfree(file_content);

        if (prev == NULL) {
            break;
        }
        file_content = container_of(prev, fat_data_t, list);
    }
}

void write_file(fs_t* fs, dev_t* dev, file_t* file, byte* buffer, size_t count) {
    struct fat_info* fat_info = fs->fs_data;
    fat_file_data_t* file_data = file->fs_data;

    fat_data_t* file_content = fat_read_data(dev, fat_info, file_data->cluster);

    size_t position = file->position;
    size_t wrote_bytes = 0;

    while (1) {
        for (size_t i = 0; i < fat_info->cluster_size; i++) {
            *(byte*)(file_content->entry + position % fat_info->cluster_size) = *(buffer + wrote_bytes);
            wrote_bytes++;
            position++;
            if (wrote_bytes >= count) {
                break;
            }
        }

        list_t* prev = file_content->list.prev;

        if (prev == NULL) {
            break;
        }
        file_content = container_of(prev, fat_data_t, list);
    }
    
    fat_write_data(dev, fat_info, file_data->cluster, file_content);
}

void create_file(fs_t* fs, dev_t* dev, dir_t* dir, char* name) {
    
}

void delete_file(fs_t* fs, dev_t* dev, dir_t* dir, char* name) {
    
}

void create_dir(fs_t* fs, dev_t* dev, dir_t* parent, char* name) {
    
}

void delete_dir(fs_t* fs, dev_t* dev, dir_t* parent, char *name) {
    
}