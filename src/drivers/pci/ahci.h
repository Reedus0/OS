#pragma once

#include <stdint.h>
#include <stddef.h>

// Reference: https://www.intel.com/content/dam/www/public/us/en/documents/technical-specifications/serial-ata-ahci-spec-rev1-3-1.pdf

#include "include/module.h"
#include "include/dev.h"
#include "pci.h"

#define	SATA_SIG_ATA 0x00000101

#define AHCI_DEFAULT_VENDOR 0x8086
#define AHCI_DEFAULT_DEVICE_ID 0x2922

#define HBA_CMD_ST 1
#define HBA_CMD_FRE 16

struct hba_port {
    uint32_t clb;
    uint32_t clbu;
    uint32_t fb;
    uint32_t fbu;
    uint32_t is;
    uint32_t ie;
    uint32_t cmd;
    uint32_t reserved_1;
    uint32_t tfd;
    uint32_t sig;
    uint32_t ssts;
    uint32_t sctl;
    uint32_t serr;
    uint32_t sact;
    uint32_t ci;
    uint32_t sntf;
    uint32_t fbs;
    uint32_t reserved_2[11];
    uint32_t vendor[4];
};
typedef struct hba_port hba_port_t;

struct ahci_registers {
    uint32_t cap;
    uint32_t ghc;
    uint32_t is;
    uint32_t pi;
    uint32_t vs;
    uint32_t ccc_ctl;
    uint32_t ccc_pts;
    uint32_t em_loc;
    uint32_t em_ctl;
    uint32_t cap2;
    uint32_t bohc;
    uint8_t reserved_1[116];
    uint8_t vendor[96];
    hba_port_t ports[32];
} __attribute__((packed));
typedef struct ahci_registers ahci_registers_t;

struct ahci_data {
    uint16_t vendor;
    uint16_t device_id;
    pci_device_t* pci_device;
    ahci_registers_t* registers;
};
typedef struct ahci_data ahci_data_t;

struct ahci_drive_data {
    dev_t* ahci;
    hba_port_t* port;
};
typedef struct ahci_drive_data ahci_drive_data_t;

module_t* init_ahci_module();
dev_t* init_ahci_dev(uint16_t vendor, uint16_t device_id);
void deinit_ahci_dev(dev_t* ahci_dev);
dev_t* init_ahci_drive_dev(dev_t* ahci, uint8_t port);
void deinit_ahci_drive_dev(dev_t* ahci_drive_dev);