#pragma once

#include <stdint.h>
#include <stddef.h>

#include "include/dev.h"

struct fat_bpb_16 {
    uint8_t drv_num_16;
    uint8_t reserved_1;
    uint8_t boot_sig_16;
    uint32_t vol_id_16;
    uint8_t vol_lab_16[11];
    uint64_t fil_sys_type_16;
} __attribute__((packed));

struct fat_bpb_32 {
    uint32_t fat_sz_32;
    uint16_t ext_flags;
    uint16_t fs_ver;
    uint32_t root_clus;
    uint16_t fs_info;
    uint16_t bk_boot_sec;
    uint8_t reserved_3[12];
    uint8_t drv_num_32;
    uint8_t reserved_4;
    uint8_t boot_sig_32;
    uint32_t vol_id_32;
    uint8_t vol_lab_32[11];
    uint64_t fil_sys_type_32;
} __attribute__((packed));

struct fat_bpb {
    uint8_t reserved_1[3];
    uint8_t oem_name[8];
    uint16_t byts_per_sec;
    uint8_t sec_per_clus;
    uint16_t rsvd_sec_cnt;
    uint8_t num_fats;
    uint16_t root_ent_cnt;
    uint16_t tot_sec_16;
    uint8_t media;
    uint16_t fat_sz_16;
    uint16_t sec_per_trk;
    uint16_t num_heads;
    uint32_t hidd_sec;
    uint32_t tot_sec_32;
    union {
        struct fat_bpb_16 fat_bpb_16;
        struct fat_bpb_32 fat_bpb_32;
    };
} __attribute__((packed));

struct fat_bpb* get_bpb(dev_t* dev);