#pragma once

#include <stdint.h>
#include <stddef.h>

// Reference: https://wiki.osdev.org/PCI

#include "include/module.h"
#include "kernel/shell.h"

#define PCI_DEFAULT_CONFIG_ADDRESS 0xCF8
#define PCI_DEFAULT_CONFIG_DATA 0xCFC

#define PCI_VENDOR 0
#define PCI_DEVICE 2
#define PCI_COMMAND 4
#define PCI_STATUS 6
#define PCI_REVISION 8
#define PCI_PROG_IF 9
#define PCI_SUBCLASS 10
#define PCI_CLASS 11
#define PCI_CACHE_SIZE 12
#define PCI_LATENCE_TIMER 13
#define PCI_HEADER_TYPE 14
#define PCI_BIST 15

#define PCI_DEVICE_BASE_ADDRESS_0 16
#define PCI_DEVICE_BASE_ADDRESS_1 20
#define PCI_DEVICE_BASE_ADDRESS_2 24
#define PCI_DEVICE_BASE_ADDRESS_3 28
#define PCI_DEVICE_BASE_ADDRESS_4 32
#define PCI_DEVICE_BASE_ADDRESS_5 36
#define PCI_DEVICE_CIS_POINTER 40
#define PCI_DEVICE_SUBSYSTEM_VENDOR 44
#define PCI_DEVICE_SUBSYSTEM 46
#define PCI_DEVICE_EXPANSION_ROM 48
#define PCI_DEVICE_CAPABILITIES 52
#define PCI_DEVICE_INTERRUPT_LINE 60
#define PCI_DEVICE_INTERRUPT_PIN 61
#define PCI_DEVICE_MIN_GRANT 62
#define PCI_DEVICE_MAX_LATENCY 63

#define PCI_BRIDGE_BASE_ADDRESS_0 16
#define PCI_BRIDGE_BASE_ADDRESS_1 20
#define PCI_BRIDGE_PRIMARY_BUS 24
#define PCI_BRIDGE_SECONDARY_BUS 25

#define PCI_SET_PORTS 0
#define PCI_CHECK_BUSES 1
#define PCI_DEVICE_GET_VALUE 2
#define PCI_DEVICE_SET_VALUE 3
#define PCI_DEVICE_SET_MASTER 4 
#define PCI_DEVICE_CLEAR_MASTER 5

struct pci_device {
    uint16_t vendor;
    uint16_t device_id;

    uint8_t bus;
    uint8_t device;
    uint8_t function;
};
typedef struct pci_device pci_device_t;

pci_device_t g_pci_devices[16];
size_t g_pci_devices_count;

module_t g_pci_module;
module_t init_pci_module();
shell_command sh_pci(char* command);