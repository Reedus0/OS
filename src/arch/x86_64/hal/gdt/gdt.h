#pragma once

#include <stdint.h>
#include <stddef.h>

// Reference https://wiki.osdev.org/Global_Descriptor_Table

#define GDT_DESCRIPTORS_COUNT 5

struct gdt_descriptor {
    uint32_t reserved_1;
    uint8_t reserved_2;
    uint8_t access_byte;
    uint8_t flags;
    uint8_t reserved_3;
} __attribute__((packed));
typedef struct gdt_descriptor gdt_descriptor_t;

gdt_descriptor_t g_gdt_descriptors[GDT_DESCRIPTORS_COUNT];

struct gdt {
    uint16_t size;
    uint64_t offset;
} __attribute__((packed));
typedef struct gdt gdt_t;

gdt_t g_gdt;

void init_gdt();