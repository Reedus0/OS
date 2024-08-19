#include "pci.h"
#include "include/io.h"

static uint16_t config_address_port;
static uint16_t config_data_port;

static uint16_t pci_read(uint8_t bus, uint8_t slot, uint8_t function, uint8_t offset) {
    uint32_t address = (
        (uint32_t)((uint32_t)bus << 16) |
        ((uint32_t)slot << 11) |
        ((uint32_t)function << 8) |
        (offset & 0xFC) |
        ((uint32_t)1 << 31)
    );
    out32(config_address_port, address);
    uint16_t result = (in32(config_data_port) >> ((offset & 2) * 8)) & 0xFFFF;
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
    out32(config_address_port, address);
    out32(config_data_port, data);
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

static void add_pci_device(uint8_t bus, uint8_t device) {
    pci_device_t pci_device;
    pci_device.vendor = pci_get_value16(bus, device, 0, PCI_VENDOR);
    pci_device.device_id = pci_get_value16(bus, device, 0, PCI_DEVICE);

    pci_device.bus = bus,
    pci_device.device = device;

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
}

static void pci_check_device(uint8_t bus, uint8_t device) {
    uint16_t vendor = pci_get_value16(bus, device, 0, PCI_VENDOR);
    if (vendor == 0xFFFF) {
        return;
    }
    pci_check_function(bus, device, 0);

    add_pci_device(bus, device);
    
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

static uint32_t pci_device_get_value(pci_device_t pci_device, uint8_t function, uint8_t offset) {
    return pci_get_value32(pci_device.bus, pci_device.device, function, offset);
}

static void pci_device_set_value(pci_device_t pci_device, uint8_t function,  uint8_t offset, uint32_t data) {
    pci_write(pci_device.bus, pci_device.device, function, offset, data);
}

static void pci_set_ports(uint16_t config_address, uint16_t config_data) {
    config_address_port = config_address;
    config_data_port = config_data;
}

static void init_pci() {
    config_address_port = PCI_DEFAULT_CONFIG_ADDRESS;
    config_data_port = PCI_DEFAULT_CONFIG_DATA;
}

static void deinit_pci() {

}

module_t init_pci_module() {
    if(g_pci_module.deinit != NULL) {
        g_pci_module.deinit();
    }
    g_pci_module.name = "PCI module";

    g_pci_module.init = init_pci;

    MODULE_FUNCTION(g_pci_module, PCI_SET_PORTS) = pci_set_ports;
    MODULE_FUNCTION(g_pci_module, PCI_CHECK_BUSES) = pci_check_buses;
    MODULE_FUNCTION(g_pci_module, PCI_DEVICE_GET_VALUE) = pci_device_get_value;
    MODULE_FUNCTION(g_pci_module, PCI_DEVICE_SET_VALUE) = pci_device_set_value;

    g_pci_module.deinit = deinit_pci;

    return g_pci_module;
}

shell_command sh_pci(char* command) {
    for (size_t i = 0; i < g_pci_devices_count; i++) {
        if (i > 16) break;
        pci_device_t current_device = g_pci_devices[i];
        printk("DEVICE: %x:%x \n",
        current_device.vendor, current_device.device_id);
    }
    return 0;
}