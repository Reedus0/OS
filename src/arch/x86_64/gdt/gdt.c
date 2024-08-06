#include "gdt.h"
#include "lgdt.h"

static void gdt_descriptor_set_access_byte(struct gdt_descriptor* gdt_descriptor, uint8_t access_byte) {
    gdt_descriptor->access_byte = access_byte;
}

static void gdt_descriptor_set_flags(struct gdt_descriptor* gdt_descriptor, uint8_t flags) {
    gdt_descriptor->flags = flags;
}

void setup_gdt() {
    g_kernel_code_segment = g_gdt_descriptors[1];
    gdt_descriptor_set_access_byte(&g_kernel_code_segment, 0x9a);
    gdt_descriptor_set_flags(&g_kernel_code_segment, 0xa);

    g_kernel_data_segment = g_gdt_descriptors[2];
    gdt_descriptor_set_access_byte(&g_kernel_data_segment, 0x92);
    gdt_descriptor_set_flags(&g_kernel_data_segment, 0xc);

    g_user_code_segment = g_gdt_descriptors[3];
    gdt_descriptor_set_access_byte(&g_user_code_segment, 0xfa);
    gdt_descriptor_set_flags(&g_user_code_segment, 0xa);

    g_user_data_segment = g_gdt_descriptors[4];
    gdt_descriptor_set_access_byte(&g_user_data_segment, 0xf2);
    gdt_descriptor_set_flags(&g_user_data_segment, 0xc);

    g_gdt.size = GDT_DESCRIPTORS_COUNT * sizeof(struct gdt_descriptor) - 1;
    g_gdt.offset = &g_gdt_descriptors;

    load_gdt(&g_gdt);
}