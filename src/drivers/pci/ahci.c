#include "ahci.h"
#include "pci.h"
#include "asm/io.h"

static pci_device_t* ahci_device;
static uint32_t ahci_registers_address = 0xFEA00000;

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

    // ahci_registers_address = pci_device_read(ahci_device, PCI_DEVICE_BASE_ADDRESS_5);

    pci_device_write(ahci_device, PCI_DEVICE_BASE_ADDRESS_5, ahci_registers_address);

    pci_device_enable_mmio(ahci_device);
    pci_device_take_bus(ahci_device);
    printk(NONE, "%x\n", pci_device_read(ahci_device, PCI_DEVICE_BASE_ADDRESS_5));

    for (size_t i = 0; i < 128; i++) {
        printk(NONE, "%x ", *(volatile char*)(ahci_registers_address + i));
    }

}

static void deinit_ahci() {

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