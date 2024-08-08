#include "idt.h"
#include "lidt.h"
#include "isr.h"
#include "isr_init.h"

static void idt_descriptor_set_isr(struct idt_descriptor* idt_descriptor, void (*isr)()) {
    idt_descriptor->offset_1 = (size_t)isr & 0xFFFF;
    idt_descriptor->offset_2 = ((size_t)isr >> 16) & 0xFFFF;
    idt_descriptor->offset_3 = ((size_t)isr >> 32) & 0xFFFFFFFF;
}

static void idt_descriptor_set_ist(struct idt_descriptor* idt_descriptor, uint8_t ist) {
    idt_descriptor->ist = ist;
}

static void idt_descriptor_set_selector(struct idt_descriptor* idt_descriptor, uint16_t selector) {
    idt_descriptor->selector = selector;
}

static void idt_descriptor_set_type(struct idt_descriptor* idt_descriptor, uint8_t* type) {
    idt_descriptor->type = type;
}

static void idt_descriptor_enable(struct idt_descriptor* idt_descriptor) {
    idt_descriptor->type |= 1 << 7;
}

static void idt_descriptor_disable(struct idt_descriptor* idt_descriptor) {
    idt_descriptor->type &= 0 << 7;
}

void set_isr(struct idt_descriptor* idt_descriptor, void (*isr)()) {
    idt_descriptor_set_isr(idt_descriptor, isr);
}

void init_idt() {
    for (size_t i = 0; i < IDT_DESCRIPTORS_COUNT; i++) {
        struct idt_descriptor* current_idt_descriptor = &g_idt_descriptors[i];
        idt_descriptor_set_ist(current_idt_descriptor, 0);
        idt_descriptor_set_selector(current_idt_descriptor, 0 | (0 << 2) | (1 << 3));
        idt_descriptor_set_type(current_idt_descriptor, 0xe | (0 << 4));
        idt_descriptor_enable(current_idt_descriptor);
    };

    init_isrs();

    g_idt.size = IDT_DESCRIPTORS_COUNT * sizeof(struct idt_descriptor) - 1;
    g_idt.offset = &g_idt_descriptors;

    load_idt(&g_idt);
}