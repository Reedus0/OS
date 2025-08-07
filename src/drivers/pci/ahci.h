#pragma once

#include <stdint.h>
#include <stddef.h>

// Reference: https://www.intel.com/content/dam/www/public/us/en/documents/technical-specifications/serial-ata-ahci-spec-rev1-3-1.pdf

#include "include/module.h"

#define	SATA_SIG_ATA 0x00000101

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


module_t* init_ahci_module();