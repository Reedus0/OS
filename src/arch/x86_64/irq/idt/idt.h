#pragma once

#include <stdint.h>
#include <stddef.h>

// Reference https://wiki.osdev.org/Interrupt_Descriptor_Table

#define IDT_DESCRIPTORS_COUNT 256

struct idt_descriptor {
    uint16_t offset_1;
    uint16_t selector;
    uint8_t ist;
    uint8_t type;
    uint16_t offset_2;
    uint32_t offset_3;
    uint32_t reserved;  
} __attribute__((packed));

struct idt_descriptor g_idt_descriptors[IDT_DESCRIPTORS_COUNT];

struct idt {
    uint16_t size;
    uint64_t offset;
} __attribute__((packed));

struct idt g_idt;

void set_isr(struct idt_descriptor* idt_descriptor, void (*isr)());
void init_idt();