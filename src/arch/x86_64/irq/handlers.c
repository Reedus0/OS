#include "handlers.h"
#include "kernel/panic.h"

void __attribute__((__cdecl__)) irq_handler(struct irq_data irq_data) {
    if(g_interrupt_handlers[irq_data.interrupt_number] != NULL) {
        g_interrupt_handlers[irq_data.interrupt_number](&irq_data);
    } else {
        print_string("Unhandeled interrupt: ");
        print_number(irq_data.interrupt_number);
        print_string("!\n");
    }
}

static void set_irq_handler(size_t number, size_t handler) {
    g_interrupt_handlers[number] = handler;
}

interrupt irq_div_zero(struct irq_data* irq_data) {
    print_string("Exception: Division error!\n");
    panic(irq_data->regs);
}

interrupt irq_invalid_opcode(struct irq_data* irq_data) {
    print_string("Exception: Invalid opcode!\n");
    panic(irq_data->regs);
}

interrupt irq_double_fault(struct irq_data* irq_data) {
    print_string("Exception: Double fault!\n");
    panic(irq_data->regs);
}

interrupt irq_general_protection_fault(struct irq_data* irq_data) {
    print_string("Exception: General protection fault!\n");
    panic(irq_data->regs);
}

interrupt irq_timer(struct irq_data* irq_data) {
    pic_eoi(32);
}

interrupt irq_keyboard(struct irq_data* irq_data) {
    keyboard_process_key();
    pic_eoi(33);
}

void init_irq_handlers() {
    set_irq_handler(0, irq_div_zero);
    set_irq_handler(6, irq_invalid_opcode);
    set_irq_handler(8, irq_double_fault);
    set_irq_handler(13, irq_general_protection_fault);
    set_irq_handler(32, irq_timer);
    set_irq_handler(33, irq_keyboard);
}