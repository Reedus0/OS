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

#define PIC_REMAP 16
#define PIC_SEND_EOI 17
#define PIC_DISABLE 18
#define PIC_SET_PORTS 19

module_t* g_pic_module;
module_t* init_pic_module();