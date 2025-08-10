#pragma once

#include <stdint.h>
#include <stddef.h>

#include "include/module.h"
#include "include/dev.h"
#include "lib/string.h"

struct mbr_partition_table_entry {
    uint8_t boot;
    uint8_t start_head;
    uint8_t start_sector : 6;
    uint16_t start_cylinder : 10;
    uint8_t system_id;
    uint8_t end_head;
    uint8_t end_sector : 6;
    uint16_t end_cylinder : 10;
    uint32_t relative_sector;
    uint32_t total_sectors;
} __attribute__((packed));
typedef struct mbr_partition_table_entry mbr_partition_table_entry_t;

struct partition_data {
    dev_t* drive;
    uint8_t index;
    uint32_t first_sector;
    uint32_t sector_count;
};
typedef struct partition_data partition_data_t;

module_t* init_partition_module();
dev_t* init_partition_dev(dev_t* drive, uint8_t index);
void deinit_partition_dev(dev_t* partition_dev);