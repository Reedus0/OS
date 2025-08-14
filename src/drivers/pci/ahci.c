#include "ahci.h"
#include "memory/paging.h"
#include "asm/io.h"

static void ahci_drive_start_cmd(dev_t* dev) {
    ahci_drive_data_t* ahci_drive_data = dev->dev_data;

    hba_port_t* current_port = ahci_drive_data->port;

    current_port->cmd |= HBA_CMD_ST;
    current_port->cmd |= HBA_CMD_FRE;
}

static void ahci_drive_stop_cmd(dev_t* dev) {
    ahci_drive_data_t* ahci_drive_data = dev->dev_data;

    hba_port_t* current_port = ahci_drive_data->port;

    current_port->cmd &= ~HBA_CMD_ST;
    current_port->cmd &= ~HBA_CMD_FRE;
}

static void ahci_read(dev_t* dev, byte* buffer, size_t offset, size_t count) {
    ahci_drive_data_t* ahci_drive_data = dev->dev_data;
    ahci_data_t* ahci_data = ahci_drive_data->ahci->dev_data;

    ahci_drive_data->port->is = 0xFFFFFFFF;

    hba_cmd_header_t* cmd_header = (hba_cmd_header_t*)ahci_drive_data->port->clb;
    cmd_header->cfl = sizeof(fis_reg_h2d_t) / sizeof(uint32_t);
    cmd_header->w = 0;
    cmd_header->prdtl = (uint16_t)((count - 1) >> 4) + 1;

    hba_cmd_table_t* cmd_table = kalloc_aligned(sizeof(hba_cmd_table_t), 16);
    cmd_header->ctba = (uint32_t)get_physical_address(cmd_table);
    cmd_header->ctbau = (uint32_t)(get_physical_address(cmd_table) >> 32);

    uint16_t* tmp = kalloc_aligned(count * 512, 2);

    uint16_t* __tmp = get_physical_address(tmp);
    size_t __count = count;

    for (size_t i = 0; i < cmd_header->prdtl - 1; i++) {
        cmd_table->prdt_entry[i].dba = (uint32_t)__tmp;
        cmd_table->prdt_entry[i].dbau = (uint32_t)((uint64_t)__tmp >> 32);
        cmd_table->prdt_entry[i].dbc = 8192 - 1;
        cmd_table->prdt_entry[i].i = 0;

        __tmp += 4096;
        __count -= 16;
    }

    cmd_table->prdt_entry[cmd_header->prdtl - 1].dba = (uint32_t)__tmp;
    cmd_table->prdt_entry[cmd_header->prdtl - 1].dbau = (uint32_t)((uint64_t)__tmp >> 32);
    cmd_table->prdt_entry[cmd_header->prdtl - 1].dbc = (__count * 512) - 1;
    cmd_table->prdt_entry[cmd_header->prdtl - 1].i = 0;

    fis_reg_h2d_t* cmd_fis = (fis_reg_h2d_t*)get_physical_address((size_t)&cmd_table->cfis);

    cmd_fis->fis_type = FIS_TYPE_REG_H2D;
    cmd_fis->c = 1;
    cmd_fis->command = ATA_CMD_READ_DMA_EX;

    cmd_fis->lba0 = (uint8_t)offset;
    cmd_fis->lba1 = (uint8_t)(offset >> 8);
    cmd_fis->lba2 = (uint8_t)(offset >> 16);
    cmd_fis->device = 1 << 6;

    cmd_fis->lba3 = (uint8_t)(offset >> 24);
    cmd_fis->lba4 = (uint8_t)(offset >> 32);
    cmd_fis->lba5 = (uint8_t)(offset >> 40);

    cmd_fis->countl = count & 0xFF;
    cmd_fis->counth = (count >> 8) & 0xFF;

    while (ahci_drive_data->port->tfd & (ATA_DEV_BUSY | ATA_DEV_DRQ));

    ahci_drive_data->port->ci = 1;

    while (ahci_drive_data->port->ci);

    memcpy(buffer, tmp, count * 512);

    kfree(cmd_table);
    kfree(tmp);
}

static void ahci_write(dev_t* dev, byte* buffer, size_t offset, size_t count) {
    ahci_drive_data_t* ahci_drive_data = dev->dev_data;
    ahci_data_t* ahci_data = ahci_drive_data->ahci->dev_data;

    ahci_drive_data->port->is = 0xFFFFFFFF;

    hba_cmd_header_t* cmd_header = (hba_cmd_header_t*)ahci_drive_data->port->clb;
    cmd_header->cfl = sizeof(fis_reg_h2d_t) / sizeof(uint32_t);
    cmd_header->w = 1;
    cmd_header->prdtl = (uint16_t)((count - 1) >> 4) + 1;

    hba_cmd_table_t* cmd_table = kalloc_aligned(sizeof(hba_cmd_table_t), 16);
    cmd_header->ctba = (uint32_t)get_physical_address(cmd_table);
    cmd_header->ctbau = (uint32_t)(get_physical_address(cmd_table) >> 32);

    uint16_t* tmp = kalloc_aligned(count * 512, 2);
    memcpy(tmp, buffer, count * 512);

    uint16_t* __tmp = get_physical_address(tmp);
    size_t __count = count;

    for (size_t i = 0; i < cmd_header->prdtl - 1; i++) {
        cmd_table->prdt_entry[i].dba = (uint32_t)__tmp;
        cmd_table->prdt_entry[i].dbau = (uint32_t)((uint64_t)__tmp >> 32);
        cmd_table->prdt_entry[i].dbc = 8192 - 1;
        cmd_table->prdt_entry[i].i = 0;

        __tmp += 4096;
        __count -= 16;
    }

    cmd_table->prdt_entry[cmd_header->prdtl - 1].dba = (uint32_t)__tmp;
    cmd_table->prdt_entry[cmd_header->prdtl - 1].dbau = (uint32_t)((uint64_t)__tmp >> 32);
    cmd_table->prdt_entry[cmd_header->prdtl - 1].dbc = (__count * 512) - 1;
    cmd_table->prdt_entry[cmd_header->prdtl - 1].i = 0;

    fis_reg_h2d_t* cmd_fis = (fis_reg_h2d_t*)get_physical_address((size_t)&cmd_table->cfis);

    cmd_fis->fis_type = FIS_TYPE_REG_H2D;
    cmd_fis->c = 1;
    cmd_fis->command = ATA_CMD_WRITE_DMA_EX;

    cmd_fis->lba0 = (uint8_t)offset;
    cmd_fis->lba1 = (uint8_t)(offset >> 8);
    cmd_fis->lba2 = (uint8_t)(offset >> 16);
    cmd_fis->device = 1 << 6;

    cmd_fis->lba3 = (uint8_t)(offset >> 24);
    cmd_fis->lba4 = (uint8_t)(offset >> 32);
    cmd_fis->lba5 = (uint8_t)(offset >> 40);

    cmd_fis->countl = count & 0xFF;
    cmd_fis->counth = (count >> 8) & 0xFF;

    while (ahci_drive_data->port->tfd & (ATA_DEV_BUSY | ATA_DEV_DRQ));

    ahci_drive_data->port->ci = 1;

    while (ahci_drive_data->port->ci);

    kfree(cmd_table);
    kfree(tmp);
}

static size_t ahci_get_block_size(dev_t* dev) {
    return 512;
}

static void init_ahci(dev_t* dev) {
    ahci_data_t* ahci_data = dev->dev_data;

    for (size_t i = 0; i < g_pci_devices_count; i++) {
        pci_device_t* current_device = &g_pci_devices[i];
        if (current_device->vendor == ahci_data->vendor && current_device->device_id == ahci_data->device_id) {
            ahci_data->pci_device = current_device;
            break;
        }
    }

    if (!ahci_data->pci_device) return;

    ahci_data->registers = pci_device_request_region(ahci_data->pci_device, 5);
}

static void deinit_ahci(dev_t* dev) {

}

module_t* init_ahci_module() {
    module_t* ahci_module = kalloc(sizeof(module_t));

    ahci_module->name = "AHCI module";

    ahci_module->init = init_ahci;

    ahci_module->deinit = deinit_ahci;

    return ahci_module;
}

dev_t* init_ahci_dev(uint16_t vendor, uint16_t device_id) {
    dev_t* ahci_dev = kalloc(sizeof(dev_t));

    ahci_data_t* ahci_data = kalloc(sizeof(ahci_data_t));

    ahci_data->vendor = vendor;
    ahci_data->device_id = device_id;

    ahci_dev->dev_data = ahci_data;
    ahci_dev->driver = init_ahci_module(ahci_dev);
    ahci_dev->driver->init(ahci_dev);

    return ahci_dev;
}

void deinit_ahci_dev(dev_t* ahci_dev) {
    ahci_dev->driver->deinit(ahci_dev);

    kfree(ahci_dev->dev_data);
    kfree(ahci_dev);
}

static void init_ahci_drive(dev_t* dev) {
    ahci_drive_data_t* ahci_drive_data = dev->dev_data;

    hba_port_t* current_port = ahci_drive_data->port;

    ahci_drive_stop_cmd(dev);

    if (current_port->sig == SATA_SIG_ATA && current_port->ssts > 1) {
        void* clb = kalloc_aligned(1024, 1024);
        void* fb = kalloc_aligned(256, 256);

        // ahci_drive_data->clb = clb;
        // ahci_drive_data->fb = fb;

        current_port->clb = (uint32_t)get_physical_address(clb);
        current_port->clbu = (uint32_t)((uint64_t)get_physical_address(clb) >> 32);
        current_port->fb = (uint32_t)get_physical_address(fb);
        current_port->fbu = (uint32_t)((uint64_t)get_physical_address(fb) >> 32);
    }

    ahci_drive_start_cmd(dev);
}

static void deinit_ahci_drive(dev_t* dev) {
    ahci_drive_data_t* ahci_drive_data = dev->dev_data;

    hba_port_t* current_port = ahci_drive_data->port;

    // kfree(ahci_drive_data->clb);
    // kfree(ahci_drive_data->fb);
}

module_t* init_ahci_drive_module() {
    module_t* ahci_drive_module = kalloc(sizeof(module_t));

    ahci_drive_module->name = "AHCI drive module";

    ahci_drive_module->init = init_ahci_drive;

    MODULE_FUNCTION(ahci_drive_module, BDEV_DRIVER_READ_BLOCK) = ahci_read;
    MODULE_FUNCTION(ahci_drive_module, BDEV_DRIVER_WRITE_BLOCK) = ahci_write;
    MODULE_FUNCTION(ahci_drive_module, BDEV_DRIVER_GET_BLOCK_SIZE) = ahci_get_block_size;

    ahci_drive_module->deinit = deinit_ahci_drive;

    return ahci_drive_module;
}

dev_t* init_ahci_drive_dev(dev_t* ahci, uint8_t port) {
    dev_t* ahci_drive_dev = kalloc(sizeof(dev_t));

    ahci_drive_data_t* ahci_drive_data = kalloc(sizeof(ahci_drive_data_t));

    ahci_drive_data->ahci = ahci;

    ahci_data_t* ahci_data = ahci_drive_data->ahci->dev_data;
    ahci_drive_data->port = &ahci_data->registers->ports[port];

    ahci_drive_dev->dev_data = ahci_drive_data;
    ahci_drive_dev->driver = init_ahci_drive_module(ahci_drive_dev);
    ahci_drive_dev->driver->init(ahci_drive_dev);

    return ahci_drive_dev;
}

void deinit_ahci_drive_dev(dev_t* ahci_drive_dev) {
    ahci_drive_dev->driver->deinit(ahci_drive_dev);

    kfree(ahci_drive_dev->dev_data);
    kfree(ahci_drive_dev);
}