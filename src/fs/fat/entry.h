#pragma once

#include <stdint.h>
#include <stddef.h>

struct fat_entry {
    uint8_t name[11];
    uint8_t attributes;
    uint8_t reserved_1;
    uint8_t creation_mills;
    uint16_t creation_time;
    uint16_t creation_date;
    uint16_t access_date;
    uint16_t cluster_high;
    uint16_t modification_time;
    uint16_t modification_date;
    uint16_t cluster_low;
    uint32_t size;
} __attribute__((packed));
typedef struct fat_entry fat_entry_t;

struct fat_long_name {
    uint8_t order;
    uint8_t name_1[10];
    uint8_t attribute;
    uint8_t entry_type;
    uint8_t checksum;
    uint8_t name_2[12];
    uint8_t reserved_1;
    uint8_t name_3[2];
} __attribute__((packed));
typedef struct fat_long_name fat_long_name_t;

char* fat_entry_get_lfn(fat_entry_t* fat_entry);