#include "ide.h"
#include "include/kalloc.h"
#include "include/io.h"

static void read_bytes(uint8_t port, byte* buffer, size_t count) {
    size_t i = 0;
    while (i < count * 512) {
        while (in8(IDE_STATUS_REGISTER(port)) == 0xD0);

        for (size_t j = 0; j < 256; j++) {
            uint16_t current_word = in16(IDE_DATA_REGISTER(port));
            printk(NONE, "%x ", current_word);
            buffer[i] = (uint8_t)current_word;
            buffer[i + 1] = (uint8_t)(current_word >> 8);
            i += 2;
        }
        io_wait();
    }
}

static void ide_read_sectors(dev_t* dev, byte* buffer, size_t offset, size_t count) {
    ide_data_t* ide_data = dev->dev_data;
    while (in8(IDE_STATUS_REGISTER(ide_data->port)) != 0x50);

    out8(IDE_HEAD_REGISTER(ide_data->port), ide_data->drive | (0xE0 << 4) | (((uint8_t)(offset >> 24)) & 0x0F));
    out8(IDE_SECTOR_COUNT_REGISTER(ide_data->port), count);
    out8(IDE_LBA_LOW_REGISTER(ide_data->port), (uint8_t)offset);
    out8(IDE_LBA_MID_REGISTER(ide_data->port), (uint8_t)(offset >> 8));
    out8(IDE_LBA_HIGH_REGISTER(ide_data->port), (uint8_t)(offset >> 16));

    out8(IDE_COMMAND_REGISTER(ide_data->port), 0x20);

    read_bytes(ide_data->port, buffer, count);

    while (in8(IDE_STATUS_REGISTER(ide_data->port)) != 0x50);
}

static void write_bytes(uint8_t port, byte* buffer, size_t count) {
    size_t i = 0;
    while (i < count * 512) {
        while (in8(IDE_STATUS_REGISTER(port)) == 0xD0);
        for (size_t j = 0; j < 256; j++) {
            uint16_t result = (uint8_t)buffer[i] | (uint16_t)(buffer[i + 1]) << 8;
            out16(IDE_DATA_REGISTER(port), result);
            io_wait();
            i += 2;
        }
        out8(IDE_COMMAND_REGISTER(port), 0xE7);
    }
}

static void ide_write_sectors(dev_t* dev, byte* buffer, size_t offset, size_t count) {
    ide_data_t* ide_data = dev->dev_data;
    while (in8(IDE_STATUS_REGISTER(ide_data->port)) != 0x50);

    out8(IDE_HEAD_REGISTER(ide_data->port), ide_data->drive | (0xE0 << 4) | (((uint8_t)(offset >> 24)) & 0x0F));
    out8(IDE_SECTOR_COUNT_REGISTER(ide_data->port), count);
    out8(IDE_LBA_LOW_REGISTER(ide_data->port), (uint8_t)offset);
    out8(IDE_LBA_MID_REGISTER(ide_data->port), (uint8_t)(offset >> 8));
    out8(IDE_LBA_HIGH_REGISTER(ide_data->port), (uint8_t)(offset >> 16));
    out8(IDE_COMMAND_REGISTER(ide_data->port), 0x30);

    write_bytes(ide_data->port, buffer, count);

    while (in8(IDE_STATUS_REGISTER(ide_data->port)) != 0x50);
}

static size_t ide_get_block_size(dev_t* dev) {
    return 512;
}

static void init_ide(dev_t* dev) {

}

static void deinit_ide(dev_t* dev) {

}

module_t* init_ide_module() {
    module_t* ide_module = kalloc(sizeof(module_t));

    ide_module->name = "IDE module";

    ide_module->init = init_ide;

    MODULE_FUNCTION(ide_module, BDEV_DRIVER_READ_BLOCK) = ide_read_sectors;
    MODULE_FUNCTION(ide_module, BDEV_DRIVER_WRITE_BLOCK) = ide_write_sectors;
    MODULE_FUNCTION(ide_module, BDEV_DRIVER_GET_BLOCK_SIZE) = ide_get_block_size;

    ide_module->deinit = deinit_ide;

    return ide_module;
}

dev_t* init_ide_dev(uint16_t port, uint8_t drive) {
    dev_t* ide_dev = kalloc(sizeof(dev_t));

    ide_data_t* ide_data = kalloc(sizeof(ide_data_t));

    ide_data->port = port;
    ide_data->drive = drive;

    ide_dev->dev_data = ide_data;
    ide_dev->driver = init_ide_module(ide_dev);

    return ide_dev;
}

void deinit_ide_dev(dev_t* ide_dev) {
    ide_dev->driver->deinit(ide_dev);

    kfree(ide_dev->dev_data);
    kfree(ide_dev);
}