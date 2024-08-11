#include "idt.h"
#include "arch/x86_64/asm/lidt.h"
#include "isr.h"
#include "idt_init.h"

static void idt_descriptor_set_isr(idt_descriptor_t* idt_descriptor, void (*isr)()) {
    idt_descriptor->offset_1 = (size_t)isr & 0xFFFF;
    idt_descriptor->offset_2 = ((size_t)isr >> 16) & 0xFFFF;
    idt_descriptor->offset_3 = ((size_t)isr >> 32) & 0xFFFFFFFF;
}

static void idt_descriptor_set_ist(idt_descriptor_t* idt_descriptor, uint8_t ist) {
    idt_descriptor->ist = ist;
}

static void idt_descriptor_set_selector(idt_descriptor_t* idt_descriptor, uint16_t selector) {
    idt_descriptor->selector = selector;
}

static void idt_descriptor_set_type(idt_descriptor_t* idt_descriptor, uint8_t* type) {
    idt_descriptor->type = type;
}

static void idt_descriptor_enable(idt_descriptor_t* idt_descriptor) {
    idt_descriptor->type |= 1 << 7;
}

static void idt_descriptor_disable(idt_descriptor_t* idt_descriptor) {
    idt_descriptor->type ^= ((idt_descriptor->type >> 7) & 1);
}

static void init_interrupt(idt_descriptor_t* idt_descriptor) {
    idt_descriptor_set_ist(idt_descriptor, 0);
    idt_descriptor_set_selector(idt_descriptor, 0 | (0 << 2) | (1 << 3));
    idt_descriptor_set_type(idt_descriptor, 0xE | (0 << 4));
    idt_descriptor_enable(idt_descriptor);
}

static void init_trap(idt_descriptor_t* idt_descriptor) {
    idt_descriptor_set_ist(idt_descriptor, 0);
    idt_descriptor_set_selector(idt_descriptor, 0 | (0 << 2) | (1 << 3));
    idt_descriptor_set_type(idt_descriptor, 0xF | (0 << 4));
    idt_descriptor_enable(idt_descriptor);
}

void set_isr(idt_descriptor_t* idt_descriptor, void (*isr)()) {
    idt_descriptor_set_isr(idt_descriptor, isr);
}

void init_idt() {
    for (size_t i = 0; i < IDT_DESCRIPTORS_COUNT; i++) {
        idt_descriptor_t* current_idt_descriptor = &g_idt_descriptors[i];
        if (i != 14) {
            init_interrupt(current_idt_descriptor);
        } else {
            init_trap(current_idt_descriptor);
        }
    };

    init_isrs();

    g_idt.size = IDT_DESCRIPTORS_COUNT * sizeof(idt_descriptor_t) - 1;
    g_idt.offset = &g_idt_descriptors;

    load_idt(&g_idt);
}