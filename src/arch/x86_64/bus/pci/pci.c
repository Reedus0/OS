#include <stdlib.h>
#include "pci.h"
#include "arch/x86_64/asm/io.h"

static uint16_t pci_read_word(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
    uint32_t address = (
        (uint32_t)((uint32_t)bus << 16) |
        ((uint32_t)slot << 11) |
        ((uint32_t)func << 8) |
        (offset & 0xFC) |
        ((uint32_t)1 << 31)
    );
    outd(PCI_CONFIG_ADDRESS, address);
    uint16_t result = (uint16_t)((ind(PCI_CONFIG_DATA) >> ((offset & 2) * 8)) & 0xFFFF);
    return result;
}

static char* info_pci() {
    printf("%x\n", pci_read_word(0, 1, 0, 0));
    printf("%x\n", pci_read_word(0, 1, 0, 2));
    printf("%x\n", pci_read_word(0, 1, 0, 4));
    printf("%x\n", pci_read_word(0, 1, 0, 6));
    printf("%x\n", pci_read_word(0, 1, 0, 8));
    printf("%x\n", pci_read_word(0, 1, 0, 10));
    return "PCI driver";
}

static void init_pci() {
    
}

static void deinit_pci() {

}

driver_t init_pci_driver() {
    if(g_pci_driver.deinit != NULL) {
        g_pci_driver.deinit();
    }
    g_pci_driver.name = "PCI driver";
    g_pci_driver.info = info_pci;

    g_pci_driver.init = init_pci;

    driver_function(g_pci_driver, PCI_READ_WORD) = pci_read_word;

    g_pci_driver.deinit = deinit_pci;

    return g_pci_driver;
}