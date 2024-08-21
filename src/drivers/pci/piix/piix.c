#include "piix.h"
#include "include/io.h"
#include "drivers/bus/pci/pci.h"

static pci_device_t piix_ide;
static uint16_t base_address;

static pci_device_t find_piix_ide() {
    for (size_t i = 0; i < g_pci_devices_count; i++) {
        if (i > 16) break;
        pci_device_t current_device = g_pci_devices[i];
        if (current_device.vendor == 0x8086 && current_device.device_id == 0x7010) {
            return current_device;
        }
    }
}

void init_piix() {
    base_address = PIIX_DEFAULT_BASE_ADDRESS;
    piix_ide = find_piix_ide();
}

void deinit_piix() {
    
}

module_t init_piix_module() {
    if (g_piix_module.deinit != NULL) {
        g_piix_module.deinit();
    }
    g_piix_module.name = "PIIX module";

    g_piix_module.init = init_piix;

    g_piix_module.deinit = deinit_piix;

    return g_piix_module;
}