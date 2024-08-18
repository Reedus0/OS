#include "pci.h"
#include "arch/x86_64/asm/io.h"
#include "include/types.h"

static uint16_t pci_read(uint8_t bus, uint8_t slot, uint8_t function, uint8_t offset) {
    uint32_t address = (
        (uint32_t)((uint32_t)bus << 16) |
        ((uint32_t)slot << 11) |
        ((uint32_t)function << 8) |
        (offset & 0xFC) |
        ((uint32_t)1 << 31)
    );
    out32(PCI_CONFIG_ADDRESS, address);
    uint16_t result = (in32(PCI_CONFIG_DATA) >> ((offset & 2) * 8)) & 0xFFFF;
    return result;
}

static void pci_write(uint8_t bus, uint8_t slot, uint8_t function, uint8_t offset, uint32_t data) {
    uint32_t address = (
        (uint32_t)((uint32_t)bus << 16) |
        ((uint32_t)slot << 11) |
        ((uint32_t)function << 8) |
        (offset & 0xFC) |
        ((uint32_t)1 << 31)
    );
    out32(PCI_CONFIG_ADDRESS, address);
    out32(PCI_CONFIG_DATA, data);
}

static uint8_t pci_get_value8(uint8_t bus, uint8_t slot, uint8_t function, uint8_t offset) {
    uint16_t result = pci_read(bus, slot, function, offset);
    if (offset % 2 == 0) {
        return result & 0xFF;
    }
    return result >> 8;
}

static uint16_t pci_get_value16(uint8_t bus, uint8_t slot, uint8_t function, uint8_t offset) {
    return pci_read(bus, slot, function, offset);
}

static uint32_t pci_get_value32(uint8_t bus, uint8_t slot, uint8_t function, uint8_t offset) {
    uint32_t result = pci_read(bus, slot, function, offset);
    result |= pci_read(bus, slot, function, offset + 2) << 16;
    return result;
}

static void add_pci_device(pci_device_t pci_device) {
    g_pci_devices[g_pci_devices_count] = pci_device;
    g_pci_devices_count += 1;
}

static void pci_check_bus(uint8_t bus);

static void pci_check_function(uint8_t bus, uint8_t device, uint8_t function) {
    uint8_t class = pci_get_value8(bus, device, function, PCI_CLASS);
    uint8_t subclass = pci_get_value8(bus, device, function, PCI_SUBCLASS);
    if (class == 0x6 && subclass == 0x4) {
        uint8_t secondary_bus = pci_get_value8(bus, device, function, PCI_BRIDGE_SECONDARY_BUS);
        pci_check_bus(secondary_bus);
    }
    if (class != 0xFF) {
        pci_device_t pci_device;
        pci_device.vendor = pci_get_value16(bus, device, function, PCI_VENDOR);
        pci_device.device_id = pci_get_value16(bus, device, function, PCI_DEVICE);

        pci_device.bus = bus,
        pci_device.device = device;
        pci_device.function = function;

        pci_device.class_code = class;
        pci_device.subclass = subclass;
        pci_device.programming_interface = pci_get_value8(bus, device, function, PCI_PROG_IF);

        add_pci_device(pci_device);
    }
}

static void pci_check_device(uint8_t bus, uint8_t device) {
    uint16_t vendor = pci_get_value16(bus, device, 0, PCI_VENDOR);
    if (vendor == 0xFFFF) {
        return;
    }
    pci_check_function(bus, device, 0);
    uint8_t header_type = pci_get_value16(bus, device, 0, PCI_HEADER_TYPE);
    if ((header_type & 0x80) != 0) {
        for (size_t i = 1; i < 8; i++) {
            pci_check_function(bus, device, i);
        }
    }
}

static void pci_check_bus(uint8_t bus) {
    for (size_t i = 0; i < 32; i++) {
        pci_check_device(bus, i);
    }
}

static void pci_check_buses() {
    uint8_t bus_header_type = pci_get_value16(0, 0, 0, PCI_HEADER_TYPE);
    if ((bus_header_type & 0x80) == 0) { 
        pci_check_bus(0);
    }
}

static uint32_t pci_device_get_value(pci_device_t pci_device, uint8_t offset) {
    return pci_get_value32(pci_device.bus, pci_device.device, pci_device.function, offset);
}

static void pci_device_set_value(pci_device_t pci_device, uint8_t offset, uint32_t data) {
    pci_write(pci_device.bus, pci_device.device, pci_device.function, offset, data);
}

static void init_pci() {
    pci_check_buses();
}

static void deinit_pci() {

}

driver_t init_pci_driver() {
    if(g_pci_driver.deinit != NULL) {
        g_pci_driver.deinit();
    }
    g_pci_driver.name = "PCI driver";

    g_pci_driver.init = init_pci;

    driver_function(g_pci_driver, PCI_DEVICE_GET_VALUE) = pci_device_get_value;
    driver_function(g_pci_driver, PCI_DEVICE_SET_VALUE) = pci_device_set_value;

    g_pci_driver.deinit = deinit_pci;

    return g_pci_driver;
}

shell_command sh_pci(char* command) {
    for (size_t i = 0; i < g_pci_devices_count; i++) {
        if (i > 16) break;
        pci_device_t current_device = g_pci_devices[i];
        printk("DEVICE: %x:%x CLASS: %x:%x:%x \n",
        current_device.vendor, current_device.device_id,
        current_device.class_code, current_device.subclass,
        current_device.programming_interface);
    }
    return 0;
}