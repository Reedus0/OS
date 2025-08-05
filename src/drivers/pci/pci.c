#include "pci.h"
#include "include/io.h"
#include "memory/paging.h"

static uint16_t pci_read(uint8_t bus, uint8_t slot, uint8_t function, uint8_t offset) {
    uint32_t address = (
        (uint32_t)((uint32_t)bus << 16) |
        ((uint32_t)slot << 11) |
        ((uint32_t)function << 8) |
        (offset & 0xFC) |
        ((uint32_t)1 << 31)
        );
    out32(PCI_DEFAULT_CONFIG_ADDRESS, address);
    uint16_t result = (in32(PCI_DEFAULT_CONFIG_DATA) >> ((offset & 2) * 8)) & 0xFFFF;
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
    out32(PCI_DEFAULT_CONFIG_ADDRESS, address);
    out32(PCI_DEFAULT_CONFIG_DATA, data);
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

static void add_pci_device(uint8_t bus, uint8_t device, uint8_t function) {
    pci_device_t pci_device;
    uint16_t vendor = pci_get_value16(bus, device, function, PCI_VENDOR);
    uint16_t device_id = pci_get_value16(bus, device, function, PCI_DEVICE);

    if (vendor == 0xFFFF && device_id == 0xFFFF) {
        return;
    }

    pci_device.vendor = vendor;
    pci_device.device_id = device_id;

    pci_device.bus = bus;
    pci_device.device = device;
    pci_device.function = function;

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
    add_pci_device(bus, device, function);
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

static void map_pci_address_range() {
    map_page(0xFEA00000, 0xFEA00000, 0x82);
    // unmap_page(0xFEA00000);

}

void init_pci() {
    pci_check_buses();
    map_pci_address_range();
}

uint32_t pci_device_read(pci_device_t* pci_device, uint8_t offset) {
    return pci_get_value32(pci_device->bus, pci_device->device, pci_device->function, offset);
}

void pci_device_write(pci_device_t* pci_device, uint8_t offset, uint32_t data) {
    pci_write(pci_device->bus, pci_device->device, pci_device->function, offset, data);
}

void pci_device_enable_mmio(pci_device_t* pci_device) {
    uint16_t command_register = pci_device_read(pci_device, PCI_COMMAND);

    command_register &= ~1;
    command_register |= 2;

    pci_device_write(pci_device, PCI_COMMAND, command_register);
}

void pci_device_disable_mmio(pci_device_t* pci_device) {
    uint16_t command_register = pci_device_read(pci_device, PCI_COMMAND);

    command_register &= ~2;
    command_register |= 1;

    pci_device_write(pci_device, PCI_COMMAND, command_register);
}

void pci_device_take_bus(pci_device_t* pci_device) {
    uint16_t command_register = pci_device_read(pci_device, PCI_COMMAND);

    command_register |= 4;

    pci_device_write(pci_device, PCI_COMMAND, command_register);
}

void pci_device_release_bus(pci_device_t* pci_device) {
    uint16_t command_register = pci_device_read(pci_device, PCI_COMMAND);

    command_register &= ~4;

    pci_device_write(pci_device, PCI_COMMAND, command_register);
}


shell_command sh_pci(char* command) {
    for (size_t i = 0; i < g_pci_devices_count; i++) {
        if (i > 16) break;
        pci_device_t current_device = g_pci_devices[i];
        printk(NONE, "DEVICE: %x:%x \n",
            current_device.vendor, current_device.device_id);
    }
    return 0;
}