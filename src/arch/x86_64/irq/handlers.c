#include "handlers.h"
#include "exceptions.h"
#include "include/panic.h"
#include "include/timer.h"
#include "include/time.h"
#include "include/dev.h"
#include "include/task.h"
#include "include/scheduler.h"
#include "kernel/printk.h"
#include "kernel/stdin.h"
#include "drivers/pic/pic.h"

void __attribute__((__cdecl__)) irq_handler(irq_data_t irq_data) {
    uint16_t current_task = get_task_register();
    set_old_task_register(current_task);

    if (current_task != KERNEL_TASK) {
        switch_context(g_kernel_task.context);
    }
    if(g_interrupt_handlers[irq_data.interrupt_number] != NULL) {
        g_interrupt_handlers[irq_data.interrupt_number](&irq_data);
    }
    else {
        printk(NONE, "Unhandeled interrupt: 0x%x!\n", irq_data.interrupt_number);
        panic("Unhandeled interrupt");
    }
    uint16_t old_task_id = get_old_task_register();
    if (old_task_id != KERNEL_TASK) {
        task_t* old_task = get_task(old_task_id);
        switch_context(old_task->context);
    }
}

static void set_irq_handler(size_t number, size_t handler) {
    g_interrupt_handlers[number] = handler;
}

interrupt irq_ide(irq_data_t* irq_data) {
    MODULE_FUNCTION(g_pic_module, PIC_SEND_EOI)(46);
}

interrupt irq_timer(irq_data_t* irq_data) {
    g_ticks += 1;
    if (g_ticks % 10 == 0) {
        update_time();
    }
    // if (g_ticks % 100 == 0) {
    //     schedule();
    // }
    MODULE_FUNCTION(g_pic_module, PIC_SEND_EOI)(32);
}

interrupt irq_keyboard(irq_data_t* irq_data) {
    char character = sdev_read(&g_keyboard);
    if (character != NULL) {
        if (character != '\b') {
            stdin_add_byte(character);
        } else {
            stdin_delete_byte();
        }
        stdin_update();
    }
    MODULE_FUNCTION(g_pic_module, PIC_SEND_EOI)(33);
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
    set_irq_handler(46, irq_ide);
}