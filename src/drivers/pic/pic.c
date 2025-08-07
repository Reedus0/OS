#include "pic.h"
#include "include/io.h"

static void pic_remap(dev_t* dev, uint8_t offset_1, uint8_t offset_2) {
	pic_data_t* pic_data = dev->dev_data;

	uint8_t a1, a2;

	a1 = in8(pic_data->port_1 + 1);
	a2 = in8(pic_data->port_2 + 1);

	out8(pic_data->port_1, ICW1_INIT | ICW1_ICW4);
	io_wait();
	out8(pic_data->port_2, ICW1_INIT | ICW1_ICW4);
	io_wait();
	out8(pic_data->port_1 + 1, offset_1);
	io_wait();
	out8(pic_data->port_2 + 1, offset_2);
	io_wait();
	out8(pic_data->port_1 + 1, 4);
	io_wait();
	out8(pic_data->port_2 + 1, 2);
	io_wait();

	out8(pic_data->port_1 + 1, ICW4_8086);
	io_wait();
	out8(pic_data->port_2 + 1, ICW4_8086);
	io_wait();

	out8(pic_data->port_1 + 1, a1);
	out8(pic_data->port_2 + 1, a2);
}

static void pic_send_eoi(dev_t* dev, uint8_t irq) {
	pic_data_t* pic_data = dev->dev_data;

	if (irq >= 8) {
		out8(pic_data->port_2, PIC_EOI);
	}
	out8(pic_data->port_1, PIC_EOI);
}

static void pic_set_ports(dev_t* dev, uint16_t port_1, uint16_t port_2) {
	pic_data_t* pic_data = dev->dev_data;

	pic_data->port_1 = port_1;
	pic_data->port_2 = port_2;
}

static void pic_disable(dev_t* dev) {
	pic_data_t* pic_data = dev->dev_data;

	out8(pic_data->port_1 + 1, 0xff);
	out8(pic_data->port_2 + 1, 0xff);
}

static void init_pic(dev_t* dev) {

}

static void deinit_pic(dev_t* dev) {
	pic_disable(dev);
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

dev_t* init_pic_dev(uint8_t port_1, uint8_t port_2) {
	dev_t* pic_dev = kalloc(sizeof(dev_t));

	pic_data_t* pic_data = kalloc(sizeof(pic_data_t));

	pic_data->port_1 = port_1;
	pic_data->port_2 = port_2;

	pic_dev->dev_data = pic_data;
	pic_dev->driver = init_pic_module(pic_dev);

	return pic_dev;
}

void deinit_pic_dev(dev_t* pic_dev) {
	pic_dev->driver->deinit(pic_dev);

	kfree(pic_dev->dev_data);
	kfree(pic_dev);
}