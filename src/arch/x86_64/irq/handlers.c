#include "handlers.h"
#include "exceptions.h"
#include "include/panic.h"
#include "kernel/stdin.h"
#include "arch/x86_64/drivers/pic/pic.h"
#include "arch/x86_64/drivers/keyboard/keyboard.h"

void __attribute__((__cdecl__)) irq_handler(irq_data_t irq_data) {
    if(g_interrupt_handlers[irq_data.interrupt_number] != NULL) {
        g_interrupt_handlers[irq_data.interrupt_number](&irq_data);
    }
    else {
        printk("Unhandeled interrupt: 0x%x!", irq_data.interrupt_number);
    }
}

static void set_irq_handler(size_t number, size_t handler) {
    g_interrupt_handlers[number] = handler;
}

interrupt irq_timer(irq_data_t* irq_data) {
    driver_function(g_pic_driver, PIC_DRIVER_EOI)(32);
}

interrupt irq_keyboard(irq_data_t* irq_data) {
    char character = driver_function(g_keyboard_driver, KEYBOARD_DRIVER_PROCESS_KEY)();
    if (character != NULL) {
        if (character != '\b') {
            stdin_add_byte(character);
        } else {
            stdin_delete_byte();
        }
        stdin_update();
    }
    driver_function(g_pic_driver, PIC_DRIVER_EOI)(33);
}

void init_irq_handlers() {
    set_irq_handler(0, irq_handle_exception);
    set_irq_handler(1, irq_handle_exception);
    set_irq_handler(2, irq_handle_exception);
    set_irq_handler(3, irq_handle_exception);
    set_irq_handler(4, irq_handle_exception);
    set_irq_handler(5, irq_handle_exception);
    set_irq_handler(6, irq_handle_exception);
    set_irq_handler(7, irq_handle_exception);
    set_irq_handler(8, irq_handle_exception);
    set_irq_handler(9, irq_handle_exception);
    set_irq_handler(10, irq_handle_exception);
    set_irq_handler(11, irq_handle_exception);
    set_irq_handler(12, irq_handle_exception);
    set_irq_handler(13, irq_handle_exception);
    set_irq_handler(14, irq_page_fault);
    set_irq_handler(15, irq_handle_exception);
    set_irq_handler(16, irq_handle_exception);
    set_irq_handler(17, irq_handle_exception);
    set_irq_handler(18, irq_handle_exception);
    set_irq_handler(19, irq_handle_exception);
    set_irq_handler(20, irq_handle_exception);
    set_irq_handler(21, irq_handle_exception);
    set_irq_handler(22, irq_handle_exception);
    set_irq_handler(23, irq_handle_exception);
    set_irq_handler(24, irq_handle_exception);
    set_irq_handler(25, irq_handle_exception);
    set_irq_handler(26, irq_handle_exception);
    set_irq_handler(27, irq_handle_exception);
    set_irq_handler(28, irq_handle_exception);
    set_irq_handler(29, irq_handle_exception);
    set_irq_handler(30, irq_handle_exception);
    set_irq_handler(31, irq_handle_exception);
    set_irq_handler(32, irq_timer);
    set_irq_handler(33, irq_keyboard);
}