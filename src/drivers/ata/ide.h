#pragma once

#include <stdint.h>
#include <stddef.h>

// Reference: https://wiki.osdev.org/ATA_PIO_Mode

#include "include/types.h"
#include "include/module.h"

#define IDE_DEFAULT_PORT 0x1F0
#define IDE_DEFAULT_DRIVE 0xE0

#define IDE_DATA_REGISTER(port) (port)
#define IDE_ERROR_REGISTER(port) (port+1)
#define IDE_FEATURES_REGISTER(port) (port+1)
#define IDE_SECTOR_COUNT_REGISTER(port) (port+2)
#define IDE_LBA_LOW_REGISTER(port) (port+3)
#define IDE_LBA_MID_REGISTER(port) (port+4)
#define IDE_LBA_HIGH_REGISTER(port) (port+5)
#define IDE_HEAD_REGISTER(port) (port+6)
#define IDE_STATUS_REGISTER(port) (port+7)
#define IDE_COMMAND_REGISTER(port) (port+7)

#define IDE_SET_PORT 2

module_t* init_ide_module();