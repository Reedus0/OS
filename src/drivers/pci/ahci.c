#include "ahci.h"
#include "asm/io.h"

static void ahci_read(dev_t* dev, byte* buffer, size_t offset, size_t count) {
    ahci_drive_data_t* ahci_drive_data = dev->dev_data;
    ahci_data_t* ahci_data = ahci_drive_data->ahci->dev_data;

    // for (size_t i = 0; i < 256; i++) {
    //     printk(NONE, "%x ", *(((char*)ahci_drive_data->port->fbs) + i));
    // }
    // while (1);
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
    if (current_port->sig == SATA_SIG_ATA && current_port->ssts > 1) {
        current_port->clb = kalloc_aligned(1024, 1024);
        current_port->fb = kalloc_aligned(256, 256);
    }

    current_port->cmd |= HBA_CMD_ST;
    current_port->cmd |= HBA_CMD_FRE;
}

static void deinit_ahci_drive(dev_t* dev) {
    ahci_drive_data_t* ahci_drive_data = dev->dev_data;

    hba_port_t* current_port = ahci_drive_data->port;

    current_port->cmd &= ~HBA_CMD_ST;
    current_port->cmd &= ~HBA_CMD_FRE;

    kfree(current_port->clb);
    kfree(current_port->fb);
}

module_t* init_ahci_drive_module() {
    module_t* ahci_drive_module = kalloc(sizeof(module_t));

    ahci_drive_module->name = "AHCI drive module";

    ahci_drive_module->init = init_ahci_drive;

    MODULE_FUNCTION(ahci_drive_module, BDEV_DRIVER_READ_BLOCK) = ahci_read;
    MODULE_FUNCTION(ahci_drive_module, BDEV_DRIVER_WRITE_BLOCK) = NONE;
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