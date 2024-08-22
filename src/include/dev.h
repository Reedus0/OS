#pragma once

#include <stdint.h>
#include <stddef.h>

#include "include/types.h"
#include "include/module.h"

#define SDEV_DRIVER_GET_BYTE 0

#define BDEV_DRIVER_GET_BLOCK 0
#define BDEV_DRIVER_GET_BLOCK_SIZE 1

struct dev {
    module_t* driver;
};
typedef struct dev dev_t;

byte sdev_get_byte(dev_t* sdev) {
    return MODULE_FUNCTION(sdev->driver, SDEV_DRIVER_GET_BYTE)();
}

void bdev_get_block(dev_t* bdev, byte* buffer, size_t offset, size_t count) {
    MODULE_FUNCTION(bdev->driver, BDEV_DRIVER_GET_BLOCK)(buffer, offset, count);
}

size_t bdev_get_block_size(dev_t* bdev) {
    return MODULE_FUNCTION(bdev->driver, BDEV_DRIVER_GET_BLOCK_SIZE)();
}

dev_t g_keyboard;
dev_t g_hdd;