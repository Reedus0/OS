#pragma once

#include <stdint.h>
#include <stddef.h>

#include "kernel/driver.h"

#define PIC1 0x20
#define PIC2 0xA0
#define PIC1_COMMAND PIC1
#define PIC1_DATA (PIC1+1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA (PIC2+1)

#define PIC_EOI	0x20

#define ICW1_ICW4 0x01
#define ICW1_INIT 0x10

#define ICW4_8086 0x01

#define REMAP_OFFSET 32

#define PIC_DRIVER_REMAP 0
#define PIC_DRIVER_EOI 1
#define PIC_DRIVER_DISABLE 2

driver_t g_pic_driver;
driver_t init_pic_driver();