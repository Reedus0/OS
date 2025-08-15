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

#define FIS_TYPE_REG_H2D 0x27
#define FIS_TYPE_REG_D2H 0x34
#define FIS_TYPE_DMA_ACT 0x39,
#define FIS_TYPE_DMA_SETUP 0x41
#define FIS_TYPE_DATA 0x46
#define FIS_TYPE_BIST 0x58
#define FIS_TYPE_PIO_SETUP 0x5F
#define FIS_TYPE_DEV_BITS 0xA1

#define ATA_CMD_READ_DMA 0xC8
#define ATA_CMD_READ_DMA_EX 0x25
#define ATA_CMD_WRITE_DMA 0xCA
#define ATA_CMD_WRITE_DMA_EX 0x35

#define ATA_DEV_BUSY 0x80
#define ATA_DEV_DRQ 0x08

struct hba_prdt_entry {
    uint32_t dba;
    uint32_t dbau;
    uint32_t reserved_1;

    uint32_t dbc : 22;
    uint32_t reserved_2 : 9;
    uint32_t i : 1;
};
typedef struct hba_prdt_entry hba_prdt_entry_t;

struct hba_cmd_table {
    uint8_t cfis[64];
    uint8_t acmd[16];

    uint8_t reserved[48];

    hba_prdt_entry_t prdt_entry[8];
};
typedef struct hba_cmd_table hba_cmd_table_t;

struct hba_cmd_header {
    uint8_t cfl : 5;
    uint8_t a : 1;
    uint8_t w : 1;
    uint8_t p : 1;

    uint8_t r : 1;
    uint8_t b : 1;
    uint8_t c : 1;
    uint8_t reserved_1 : 1;
    uint8_t pmp : 4;

    uint16_t prdtl;

    uint32_t prdbc;

    uint32_t ctba;
    uint32_t ctbau;

    uint32_t reserved_2[4];
} __attribute__((packed));
typedef struct hba_cmd_header hba_cmd_header_t;

struct fis_reg_d2h {
    uint8_t fis_type;

    uint8_t pmport : 4;
    uint8_t reserved_1 : 2;
    uint8_t i : 1;
    uint8_t reserved_2 : 1;

    uint8_t status;
    uint8_t error;

    uint8_t lba0;
    uint8_t lba1;
    uint8_t lba2;
    uint8_t device;

    uint8_t lba3;
    uint8_t lba4;
    uint8_t lba5;
    uint8_t reserved_3;

    uint8_t countl;
    uint8_t counth;
    uint8_t reserved_4[2];

    uint8_t reserved_5[4];
} __attribute__((packed));
typedef struct fis_reg_d2h fis_reg_d2h_t;

struct fis_reg_h2d {
    uint8_t fis_type;

    uint8_t pmport : 4;
    uint8_t reserved_1 : 3;
    uint8_t c : 1;

    uint8_t command;
    uint8_t featurel;

    uint8_t lba0;
    uint8_t lba1;
    uint8_t lba2;
    uint8_t device;

    uint8_t lba3;
    uint8_t lba4;
    uint8_t lba5;
    uint8_t featureh;

    uint8_t countl;
    uint8_t counth;
    uint8_t icc;
    uint8_t control;

    uint8_t reserved_2[4];
} __attribute__((packed));
typedef struct fis_reg_h2d fis_reg_h2d_t;

struct fis_pio_setup {
    uint8_t fis_type;

    uint8_t pmport : 4;
    uint8_t reserved_1 : 1;
    uint8_t d : 1;
    uint8_t i : 1;
    uint8_t reserved_2 : 1;

    uint8_t status;
    uint8_t error;

    uint8_t lba0;
    uint8_t lba1;
    uint8_t lba2;
    uint8_t device;

    uint8_t lba3;
    uint8_t lba4;
    uint8_t lba5;
    uint8_t reserved_3;

    uint8_t countl;
    uint8_t counth;
    uint8_t reserved_4;
    uint8_t e_status;

    uint16_t tc;
    uint8_t reserved_5[2];
} __attribute__((packed));
typedef struct fis_pio_setup fis_pio_setup_t;

struct fis_dma_setup {
    uint8_t fis_type;

    uint8_t pmport : 4;
    uint8_t reserved_1 : 1;
    uint8_t d : 1;
    uint8_t i : 1;
    uint8_t a : 1;

    uint8_t reserved_2[2];

    uint64_t dma_buffer_id;
    uint32_t reserved;

    uint32_t dma_buffer_offset;
    uint32_t transfer_count;

    uint32_t reserved_3;
} __attribute__((packed));
typedef struct fis_dma_setup fis_dma_setup_t;

struct fis_dev_bits {
    uint8_t fis_type;

    uint8_t pmport : 4;
    uint8_t reserved_1 : 1;
    uint8_t i : 1;
    uint8_t n : 1;

    uint8_t status_lo : 3;
    uint8_t reserved_2 : 1;
    uint8_t status_hi : 3;
    uint8_t reserved_3 : 1;

    uint8_t error;

    uint32_t reserved_4;
} __attribute__((packed));
typedef struct fis_dev_bits fis_dev_bits_t;

struct hba_fis {
    fis_dma_setup_t	dsfis;
    uint8_t reserved_1[4];

    fis_pio_setup_t	psfis;
    uint8_t reserved_2[12];

    fis_reg_d2h_t rfis;
    uint8_t reserved_3[4];

    fis_dev_bits_t sdbfis;

    uint8_t ufis[64];

    uint8_t reserved_4[96];
} __attribute__((packed));
typedef struct hba_fis hba_fis_t;

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
} __attribute__((packed));
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
    uint8_t reserved[116];
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
    void* clb;
    void* fb;
};
typedef struct ahci_drive_data ahci_drive_data_t;

module_t* init_ahci_module();
dev_t* init_ahci_dev(uint16_t vendor, uint16_t device_id);
void deinit_ahci_dev(dev_t* ahci_dev);
dev_t* init_ahci_drive_dev(dev_t* ahci, uint8_t port);
void deinit_ahci_drive_dev(dev_t* ahci_drive_dev);