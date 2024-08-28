#include "ide.h"
#include "include/dev.h"
#include "include/kalloc.h"
#include "include/io.h"

static uint16_t ide_port;
static uint8_t ide_drive;

static void read_bytes(byte* buffer, size_t count) {
    size_t i = 0;
    while(i < count * 512) {
        while(in8(IDE_STATUS_REGISTER(ide_port)) == 0xD0);
        for (size_t j = 0; j < 256; j++) {
            uint16_t current_word = in16(IDE_DATA_REGISTER(ide_port));
            *(buffer + i) = (uint8_t)current_word;
            *(buffer + i + 1) = (uint8_t)(current_word >> 8);
            i += 2;
        }
    }
    io_wait();
}

static void ide_read_sectors(byte* buffer, size_t offset, size_t count) {
    out8(IDE_HEAD_REGISTER(ide_port), ide_drive | (0xE0 << 4) | (((uint8_t)(offset >> 24)) & 0x0F));
    out8(IDE_SECTOR_COUNT_REGISTER(ide_port), count);
    out8(IDE_LBA_LOW_REGISTER(ide_port), (uint8_t)offset);
    out8(IDE_LBA_MID_REGISTER(ide_port), (uint8_t)(offset >> 8));
    out8(IDE_LBA_HIGH_REGISTER(ide_port), (uint8_t)(offset >> 16));
    out8(IDE_COMMAND_REGISTER(ide_port), 0x20);
    
    read_bytes(buffer, count);
}

static void write_bytes(byte* buffer, size_t count) {
    size_t i = 0;
    while(i < count * 512) {
        while(in8(IDE_STATUS_REGISTER(ide_port)) == 0xD0);
        for (size_t j = 0; j < 256; j++) {
            out16(IDE_DATA_REGISTER(ide_port), (uint16_t)(*(buffer + i) | (uint16_t)(*(buffer + i + 1) << 8)));
            io_wait();
            i += 2;
        }
    }
}

static void ide_write_sectors(byte* buffer, size_t offset, size_t count) {
    out8(IDE_HEAD_REGISTER(ide_port), ide_drive | (0xE0 << 4) | (((uint8_t)(offset >> 24)) & 0x0F));
    out8(IDE_SECTOR_COUNT_REGISTER(ide_port), count);
    out8(IDE_LBA_LOW_REGISTER(ide_port), (uint8_t)offset);
    out8(IDE_LBA_MID_REGISTER(ide_port), (uint8_t)(offset >> 8));
    out8(IDE_LBA_HIGH_REGISTER(ide_port), (uint8_t)(offset >> 16));
    out8(IDE_COMMAND_REGISTER(ide_port), 0x30);
    
    write_bytes(buffer, count);

    out8(IDE_COMMAND_REGISTER(ide_port), 0xE7);
}

static size_t ide_get_block_size() {
    return 512;
}

static void ide_set_port(uint16_t port, bool slave) {
    if (slave) {
        ide_drive = 0xF0;
    } else {
        ide_drive = 0xE0;
    }
    ide_port = port;
}

static void init_ide() {
    ide_port = IDE_DEFAULT_PORT;
    ide_drive = IDE_DEFAULT_DRIVE;
}


static void deinit_ide() {
    
}

module_t* init_ide_module() {
    module_t* ide_module = kalloc(sizeof(module_t));

    ide_module->name = "IDE module";

    ide_module->init = init_ide;
    
    MODULE_FUNCTION(ide_module, BDEV_DRIVER_READ_BLOCK) = ide_read_sectors;
    MODULE_FUNCTION(ide_module, BDEV_DRIVER_WRITE_BLOCK) = ide_write_sectors;
    MODULE_FUNCTION(ide_module, BDEV_DRIVER_GET_BLOCK_SIZE) = ide_get_block_size;
    MODULE_FUNCTION(ide_module, IDE_SET_PORT) = ide_set_port;

    ide_module->deinit = deinit_ide;

    return ide_module;
}