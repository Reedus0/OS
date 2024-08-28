#include "pic.h"
#include "include/io.h"

static uint16_t pic_port_1;
static uint16_t pic_port_2;

static void pic_remap(uint8_t offset_1, uint8_t offset_2) {
    uint8_t a1, a2;

	a1 = in8(pic_port_1 + 1);
	a2 = in8(pic_port_2 + 1);

    out8(pic_port_1, ICW1_INIT | ICW1_ICW4);
	io_wait();
	out8(pic_port_2, ICW1_INIT | ICW1_ICW4);
	io_wait();
	out8(pic_port_1 + 1, offset_1);
	io_wait();
	out8(pic_port_2 + 1, offset_2);
	io_wait();
	out8(pic_port_1 + 1, 4);
	io_wait();
	out8(pic_port_2 + 1, 2);
	io_wait();
	
	out8(pic_port_1 + 1, ICW4_8086);
	io_wait();
	out8(pic_port_2 + 1, ICW4_8086);
	io_wait();

	out8(pic_port_1 + 1, a1);
	out8(pic_port_2 + 1, a2);
}

static void pic_send_eoi(uint8_t irq) {
    if(irq >= 8) {
		out8(pic_port_2, PIC_EOI);
	}
	out8(pic_port_1, PIC_EOI);
}

static void pic_set_ports(uint16_t port_1, uint16_t port_2) {
    pic_port_1 = port_1;
    pic_port_2 = port_2;
}

static void pic_disable() {
    out8(pic_port_1 + 1, 0xff);
    out8(pic_port_2 + 1, 0xff);
}

static void init_pic() {
    pic_port_1 = PIC_DEFAULT_PORT_1;
    pic_port_2 = PIC_DEFAULT_PORT_2;
}

static void deinit_pic() {
	pic_disable();
}

module_t* init_pic_module() {
	module_t* pic_module = kalloc(sizeof(module_t));

	pic_module->name = "PIC module";

	pic_module->init = init_pic;

	MODULE_FUNCTION(pic_module, PIC_REMAP) = pic_remap;
	MODULE_FUNCTION(pic_module, PIC_SEND_EOI) = pic_send_eoi;
	MODULE_FUNCTION(pic_module, PIC_DISABLE) = pic_disable;
	MODULE_FUNCTION(pic_module, PIC_SET_PORTS) = pic_set_ports;

	pic_module->deinit = deinit_pic;

	return pic_module;
}