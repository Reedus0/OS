#include "ide.h"

static uint16_t ide_port;

static void write_bytes(byte* buffer, size_t count) {
    size_t i = 0;
    while(i < count * 512) {
        while(in8(IDE_STATUS_REGISTER(ide_port)) == 0xD0);
        for (size_t j = 0; j < 256; j++) {
            uint16_t current_word = in16(IDE_DATA_REGISTER(ide_port));
            uint16_t result = (uint8_t)(current_word >> 8) | (uint8_t)current_word << 8;
            *(buffer + i) = (uint8_t)current_word;
            *(buffer + i + 1) = (uint8_t)(current_word >> 8);
            i += 2;
        }
    }
}

static void ide_read_sectors(uint16_t port, byte* buffer, size_t count, size_t offset) {
    out8(IDE_HEAD_REGISTER(ide_port), port | (0xE0 << 4) | (((uint8_t)(offset >> 24)) & 0x0F));
    out8(IDE_SECTOR_COUNT_REGISTER(ide_port), count);
    out8(IDE_LBA_LOW_REGISTER(ide_port), (uint8_t)offset);
    out8(IDE_LBA_MID_REGISTER(ide_port), (uint8_t)(offset >> 8));
    out8(IDE_LBA_HIGH_REGISTER(ide_port), (uint8_t)(offset >> 16));
    out8(IDE_COMMAND_REGISTER(ide_port), 0x20);
    
    write_bytes(buffer, count);
}

static void ide_read_sectors_master(byte* buffer, size_t count, size_t offset) {
    ide_read_sectors(0xE0, buffer, count, offset);
}

static void ide_read_sectors_slave(byte* buffer, size_t count, size_t offset) {
    ide_read_sectors(0xF0, buffer, count, offset);
}

static void init_ide() {
    ide_port = IDE_DEFAULT_PORT;
}

static void deinit_ide() {
    
}

module_t init_ide_module() {
    if (g_ide_module.deinit != NULL) {
        g_ide_module.deinit();
    }
    g_ide_module.name = "IDE module";

    g_ide_module.init = init_ide;
    
    MODULE_FUNCTION(g_ide_module, IDE_READ_SECTORS_MASTER) = ide_read_sectors_master;
    MODULE_FUNCTION(g_ide_module, IDE_READ_SECTORS_SLAVE) = ide_read_sectors_slave;

    g_ide_module.deinit = deinit_ide;

    return g_ide_module;
}