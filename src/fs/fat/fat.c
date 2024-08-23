#include "fat.h"
#include "bpb.h"
#include "include/fs.h"
#include "include/dev.h"

void fat_init(dev_t* dev, dir_t* root) {
    struct fat_bpb* bpb = get_bpb(dev);
    struct fat_data* fat_data = kalloc(sizeof(struct fat_data));

    fat_data->total_root_dir_sectors = ((bpb->root_ent_cnt * 32) + (bpb->byts_per_sec - 1)) / bpb->byts_per_sec;
    fat_data->fat_size = bpb->fat_sz_16 != 0 ? bpb->fat_sz_16 : bpb->fat_bpb_32.fat_sz_32;

    fat_data->first_data_sector = bpb->rsvd_sec_cnt + (bpb->num_fats * fat_data->fat_size) + fat_data->total_root_dir_sectors;

    fat_data->total_sectors = bpb->tot_sec_16 != 0 ? bpb->tot_sec_16 : bpb->tot_sec_32;
    fat_data->fat_offset = bpb->rsvd_sec_cnt;

    if (fat_data->total_sectors < 4085) {
        fat_data->fat_type = FAT12;
    } else if (fat_data->total_sectors < 65525) {
        fat_data->fat_type = FAT16;
    } else {
        fat_data->fat_type = FAT32;
    }

    switch (fat_data->fat_type) {
        case FAT12:
        case FAT16:
            fat_data->root_dir_sector = fat_data->first_data_sector - fat_data->total_root_dir_sectors;
            break;
        case FAT32:
            break;
    }

    printk("%x", fat_data->root_dir_sector);
    
    g_fs_fat.fs_data = fat_data;

    kfree(bpb);
}

void fat_write_file(dev_t* dev, char* path, byte* buffer, size_t count) {

}

void fat_read_file(dev_t* dev, char* path, byte* buffer, size_t count) {
    
}

void fat_create_file(dev_t* dev, char* path) {
    
}

void fat_delete_file(dev_t* dev, char* path) {
    
}

void fat_create_dir(dev_t* dev, char* path) {
    
}

void fat_delete_dir(dev_t* dev, char* path) {
    
}