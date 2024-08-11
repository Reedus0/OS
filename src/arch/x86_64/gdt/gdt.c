#include "gdt.h"
#include "arch/x86_64/asm/lgdt.h"

static void gdt_descriptor_set_access_byte(gdt_descriptor_t* gdt_descriptor, uint8_t access_byte) {
    gdt_descriptor->access_byte = access_byte;
}

static void gdt_descriptor_set_flags(gdt_descriptor_t* gdt_descriptor, uint8_t flags) {
    gdt_descriptor->flags = (flags << 4);
}

void init_gdt() {
    gdt_descriptor_t* kernel_code_segment = &g_gdt_descriptors[1];
    gdt_descriptor_set_access_byte(kernel_code_segment, 0x9a);
    gdt_descriptor_set_flags(kernel_code_segment, 0xa);

    gdt_descriptor_t* kernel_data_segment = &g_gdt_descriptors[2];
    gdt_descriptor_set_access_byte(kernel_data_segment, 0x92);
    gdt_descriptor_set_flags(kernel_data_segment, 0xa);

    gdt_descriptor_t* user_code_segment = &g_gdt_descriptors[3];
    gdt_descriptor_set_access_byte(user_code_segment, 0xfa);
    gdt_descriptor_set_flags(user_code_segment, 0xa);

    gdt_descriptor_t* user_data_segment = &g_gdt_descriptors[4];
    gdt_descriptor_set_access_byte(user_data_segment, 0xf2);
    gdt_descriptor_set_flags(user_data_segment, 0xa);

    g_gdt.size = GDT_DESCRIPTORS_COUNT * sizeof(gdt_descriptor_t) - 1;
    g_gdt.offset = &g_gdt_descriptors;

    load_gdt(&g_gdt);
}