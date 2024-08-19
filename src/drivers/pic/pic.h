#pragma once

#include <stdint.h>
#include <stddef.h>

#include "include/module.h"

#define PIC_DEFAULT_PORT_1 0x20
#define PIC_DEFAULT_PORT_2 0xA0

#define PIC_EOI	0x20
#define ICW1_ICW4 0x01
#define ICW1_INIT 0x10
#define ICW4_8086 0x01

#define PIC_REMAP 0
#define PIC_SEND_EOI 1
#define PIC_DISABLE 2
#define PIC_SET_PORTS 3

module_t g_pic_module;
module_t init_pic_module();