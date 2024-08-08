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

void pic_eoi(uint8_t irq) {
    if(irq >= 8)
		outb(PIC2_COMMAND, PIC_EOI);
	
	outb(PIC1_COMMAND, PIC_EOI);
}

void pic_disable() {
    outb(PIC1_DATA, 0xff);
    outb(PIC2_DATA, 0xff);
}

void init_pic() {
    pic_remap(REMAP_OFFSET, REMAP_OFFSET + 8);
}