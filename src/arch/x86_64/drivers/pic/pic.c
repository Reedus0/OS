#include "pic.h"
#include "arch/x86_64/io.h"

static void pic_remap(uint8_t offset_1, uint8_t offset_2) {
    uint8_t a1, a2;

	a1 = inb(PIC1_DATA);
	a2 = inb(PIC2_DATA);

    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outb(PIC1_DATA, offset_1);
	io_wait();
	outb(PIC2_DATA, offset_2);
	io_wait();
	outb(PIC1_DATA, 4);
	io_wait();
	outb(PIC2_DATA, 2);
	io_wait();
	
	outb(PIC1_DATA, ICW4_8086);
	io_wait();
	outb(PIC2_DATA, ICW4_8086);
	io_wait();

	outb(PIC1_DATA, a1);
	outb(PIC2_DATA, a2);
}

static void pic_eoi(uint8_t irq) {
    if(irq >= 8)
		outb(PIC2_COMMAND, PIC_EOI);
	
	outb(PIC1_COMMAND, PIC_EOI);
}

static void pic_disable() {
    outb(PIC1_DATA, 0xff);
    outb(PIC2_DATA, 0xff);
}

static void init_pic() {
    pic_remap(REMAP_OFFSET, REMAP_OFFSET + 8);
}

static void deinit_pic() {

}

struct driver init_pic_driver() {
	g_pic_driver.init = init_pic;

	g_pic_driver.functions[PIC_DRIVER_REMAP] = pic_remap;
	g_pic_driver.functions[PIC_DRIVER_EOI] = pic_eoi;
	g_pic_driver.functions[PIC_DRIVER_DISABLE] = pic_disable;

	g_pic_driver.deinit = deinit_pic;

	return g_pic_driver;
}