#pragma once

#include <stdint.h>
#include <stddef.h>

#include "include/module.h"
#include "include/dev.h"

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

struct pic_data {
    uint16_t port_1;
    uint16_t port_2;
};
typedef struct pic_data pic_data_t;


module_t* init_pic_module();
dev_t* init_pic_dev(uint8_t port_1, uint8_t port_2);
void deinit_pic_dev(dev_t* pic_dev);