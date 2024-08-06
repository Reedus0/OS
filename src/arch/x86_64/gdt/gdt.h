#pragma once

#include <stdint.h>
#include <stddef.h>

// Reference https://wiki.osdev.org/Global_Descriptor_Table

#define GDT_DESCRIPTORS_COUNT 5

struct gdt_descriptor {
    uint16_t limit;
    uint16_t reserved_1;
    uint8_t reserved_2;
    uint8_t access_byte;
    uint8_t flags;
    uint8_t reserved_3;
} __attribute__((packed));

struct gdt_descriptor g_gdt_descriptors[GDT_DESCRIPTORS_COUNT];
struct gdt_descriptor g_kernel_code_segment;
struct gdt_descriptor g_kernel_data_segment;
struct gdt_descriptor g_user_code_segment;
struct gdt_descriptor g_user_data_segment;

struct gdt {
    uint16_t size;
    uint64_t offset;
} __attribute__((packed));

struct gdt g_gdt;

void setup_gdt();