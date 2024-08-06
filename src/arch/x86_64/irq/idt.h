#pragma once

#include <stdint.h>
#include <stddef.h>

// Reference https://wiki.osdev.org/Interrupt_Descriptor_Table

struct idt_descriptor {
    uint16_t offset_1;
    uint16_t selector;
    uint8_t ist;
    uint8_t type;
    uint16_t offset_2;
    uint32_t offset_3;
    uint32_t reserved;  
} __attribute__((packed));

struct idt_descriptor g_idt_descriptors[256];

struct idt {
    uint16_t size;
    uint64_t offset;
} __attribute__((packed));

struct idt g_idt;