#pragma once

#include <stdint.h>
#include <stddef.h>

#define GET_CLUSTER(entry) entry->cluster_low | (uint32_t)entry->cluster_high << 16
#define VALID_ENTRY(entry) (entry->name[0] != 0)

enum FAT_ATTRIBUTES {
    READ_ONLY = 0x1,
    HIDDEN = 0x2,
    SYSTEM = 0x4,
    VALUME_ID = 0x8,
    DIRECTORY = 0x10,
    ARCHIVE = 0x20,
    LFN = 0xF,
};

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
    uint16_t reserved_1;
    uint8_t name_3[4];
} __attribute__((packed));
typedef struct fat_long_name fat_long_name_t;

char* fat_entry_read_lfn(fat_entry_t* fat_entry);
fat_entry_t* fat_entry_create(char* name, size_t cluster, enum FAT_ATTRIBUTES attributes);
void fat_entry_add(fat_entry_t* dir, fat_entry_t* fat_entry);
void fat_entry_remove(fat_entry_t* dir, size_t index);