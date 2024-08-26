#pragma once

#include <stdint.h>
#include <stddef.h>

#include "include/types.h"
#include "include/module.h"

#define SDEV_DRIVER_READ_BYTE 0
#define SDEV_DRIVER_WRITE_BYTE 1

#define BDEV_DRIVER_READ_BLOCK 2
#define BDEV_DRIVER_WRITE_BLOCK 3
#define BDEV_DRIVER_GET_BLOCK_SIZE 4

struct dev {
    module_t* driver;
};
typedef struct dev dev_t;

byte sdev_read_byte(dev_t* sdev) {
    return MODULE_FUNCTION(sdev->driver, SDEV_DRIVER_READ_BYTE)();
}

void sdev_write_byte(dev_t* sdev, byte new_byte) {
    MODULE_FUNCTION(sdev->driver, SDEV_DRIVER_WRITE_BYTE)(new_byte);
}

void bdev_read_block(dev_t* bdev, byte* buffer, size_t offset, size_t count) {
    MODULE_FUNCTION(bdev->driver, BDEV_DRIVER_READ_BLOCK)(buffer, offset, count);
}

void bdev_write_block(dev_t* bdev, byte* buffer, size_t offset, size_t count) {
    MODULE_FUNCTION(bdev->driver, BDEV_DRIVER_WRITE_BLOCK)(buffer, offset, count);
}

size_t bdev_get_block_size(dev_t* bdev) {
    return MODULE_FUNCTION(bdev->driver, BDEV_DRIVER_GET_BLOCK_SIZE)();
}

dev_t g_keyboard;
dev_t g_hdd;