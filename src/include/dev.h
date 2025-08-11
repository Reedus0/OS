#pragma once

#include <stdint.h>
#include <stddef.h>

#include "include/types.h"
#include "include/module.h"
#include "include/list.h"

#define SDEV_DRIVER_READ_BYTE 0
#define SDEV_DRIVER_WRITE_BYTE 1

#define BDEV_DRIVER_READ_BLOCK 2
#define BDEV_DRIVER_WRITE_BLOCK 3
#define BDEV_DRIVER_GET_BLOCK_SIZE 4

struct dev {
    void* dev_data;
    module_t* driver;
    list_t list;
};
typedef struct dev dev_t;

byte sdev_read(dev_t* sdev) {
    return MODULE_FUNCTION(sdev->driver, SDEV_DRIVER_READ_BYTE)(sdev);
}

void sdev_write(dev_t* sdev, byte new_byte) {
    MODULE_FUNCTION(sdev->driver, SDEV_DRIVER_WRITE_BYTE)(sdev, new_byte);
}

void bdev_read(dev_t* bdev, byte* buffer, size_t offset, size_t count) {
    if (!count) panic("Invalid count");
    if (!buffer) panic("Invalid buffer");

    MODULE_FUNCTION(bdev->driver, BDEV_DRIVER_READ_BLOCK)(bdev, buffer, offset, count);
}

void bdev_write(dev_t* bdev, byte* buffer, size_t offset, size_t count) {
    if (!count) panic("Invalid count");
    if (!buffer) panic("Invalid buffer");

    MODULE_FUNCTION(bdev->driver, BDEV_DRIVER_WRITE_BLOCK)(bdev, buffer, offset, count);
}

size_t bdev_get_block_size(dev_t* bdev) {
    return MODULE_FUNCTION(bdev->driver, BDEV_DRIVER_GET_BLOCK_SIZE)(bdev);
}

dev_t* g_dev_list;

void register_dev(dev_t* dev) {
    if (g_dev_list != NULL) {
        list_insert_after(&g_dev_list->list, &g_dev_list->list);
    }
    g_dev_list = dev;
}

void unregister_dev(dev_t* dev) {
    if (g_dev_list != NULL) {
        list_remove(&dev->list);
    }
}

dev_t* g_keyboard;
dev_t* g_terminal;
dev_t* g_pic;
dev_t* g_cmos;
dev_t* g_ahci;
dev_t* g_hdd;
dev_t* g_partition;