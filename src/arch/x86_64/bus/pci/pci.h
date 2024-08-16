#pragma once

#include <stdint.h>
#include <stddef.h>

// Reference: https://wiki.osdev.org/PCI

#include "kernel/driver.h"

#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA 0xCFC

#define PCI_READ_WORD 0

driver_t g_pci_driver;
driver_t init_pci_driver();