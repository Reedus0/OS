#include "ahci.h"
#include "pci.h"
#include "asm/io.h"

static pci_device_t* ahci_device;
static ahci_registers_t* ahci_registers;

static void ahci_read(byte* buffer, size_t offset, size_t count) {

}

static size_t ahci_get_block_size() {
    return 512;
}

static void init_ahci() {
    for (size_t i = 0; i < g_pci_devices_count; i++) {
        pci_device_t* current_device = &g_pci_devices[i];
        if (current_device->vendor == 0x8086 && current_device->device_id == 0x2922) {
            ahci_device = current_device;
            break;
        }
    }

    if (!ahci_device) return;

    ahci_registers = pci_device_request_region(ahci_device, 5);

    for (size_t i = 0; i < 32; i++) {
        hba_port_t* current_port = &ahci_registers->ports[i];
        if (current_port->sig == SATA_SIG_ATA && current_port->ssts > 1) {
            current_port->clb = kalloc_aligned(1024, 1024);
            current_port->fb = kalloc_aligned(256, 256);
        }
    }
}

static void deinit_ahci() {
    for (size_t i = 0; i < 32; i++) {
        hba_port_t* current_port = &ahci_registers->ports[i];
        if (current_port->sig == SATA_SIG_ATA && current_port->ssts > 1) {
            kfree(current_port->clb);
            kfree(current_port->fb);
        }
    }
}

module_t* init_ahci_module() {
    module_t* ahci_module = kalloc(sizeof(module_t));

    ahci_module->name = "AHCI module";

    ahci_module->init = init_ahci;

    MODULE_FUNCTION(ahci_module, BDEV_DRIVER_READ_BLOCK) = NONE;
    MODULE_FUNCTION(ahci_module, BDEV_DRIVER_WRITE_BLOCK) = NONE;
    MODULE_FUNCTION(ahci_module, BDEV_DRIVER_GET_BLOCK_SIZE) = ahci_get_block_size;

    ahci_module->deinit = deinit_ahci;

    return ahci_module;
}