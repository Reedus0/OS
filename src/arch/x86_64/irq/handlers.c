#include "handlers.h"
#include "exceptions.h"
#include "include/panic.h"
#include "include/time.h"
#include "include/dev.h"
#include "include/task.h"
#include "include/scheduler.h"
#include "kernel/printk.h"
#include "kernel/stdin.h"
#include "kernel/syscall.h"
#include "drivers/pic/pic.h"

static void save_task(struct regs* regs, irq_data_t* irq_data) {
    task_t* current_task = get_task(g_current_task_id);
    regs->rsp = irq_data->rsp;
    regs->rflags = irq_data->rflags;

    current_task->context->regs = *regs;
    current_task->context->ip = irq_data->rip;
}

static void load_task(struct regs* regs, irq_data_t* irq_data) {
    task_t* new_task = get_task(g_current_task_id);

    irq_data->rip = new_task->context->ip;
    irq_data->rsp = new_task->context->regs.rsp;

    *regs = new_task->context->regs;
}

void __attribute__((cdecl)) irq_handler(struct regs regs, irq_data_t irq_data) {
    save_task(&regs, &irq_data);

    if(g_interrupt_handlers[irq_data.interrupt_number] != NULL) {
        g_interrupt_handlers[irq_data.interrupt_number](&irq_data);
    }
    else {
        printk(NONE, "Unhandeled interrupt: 0x%x!\n", irq_data.interrupt_number);
        panic("Unhandeled interrupt");
    }

    load_task(&regs, &irq_data);
}

static void set_irq_handler(size_t number, size_t handler) {
    g_interrupt_handlers[number] = handler;
}

interrupt_t irq_ide(irq_data_t* irq_data) {
    MODULE_FUNCTION(g_pic_module, PIC_SEND_EOI)(46);
}

interrupt_t irq_timer(irq_data_t* irq_data) {
    g_ticks += 1;

    TIMER_FUNCTION(10, update_time());
    TIMER_FUNCTION(10, g_current_task_id = schedule());
    MODULE_FUNCTION(g_pic_module, PIC_SEND_EOI)(32);
}

interrupt_t irq_keyboard(irq_data_t* irq_data) {
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

interrupt_t irq_syscall(irq_data_t* irq_data) {
    task_t* current_task = get_task(g_current_task_id);
    // current_task->status = NOT_READY;
    struct regs* regs = &current_task->context->regs;

    // task_t* syscall_task = create_task(syscall, NULL);

    // syscall_task->context->regs.rdi = regs->rdi;
    // syscall_task->context->regs.rsi = regs->rsi;
    // syscall_task->context->regs.rdx = regs->rdx;
    // syscall_task->context->regs.rcx = regs->rcx;

    task_t* syscall_task = create_task(g_syscalls[regs->rax], NULL);

    syscall_task->context->regs.rdi = regs->rsi;
    syscall_task->context->regs.rsi = regs->rdx;
    syscall_task->context->regs.rdx = regs->rcx;
    syscall_task->context->regs.rcx = regs->r8;
    
    schedule_task(syscall_task);
    g_current_task_id = schedule();

    // print_regs(&syscall_task->context->regs);
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
    set_irq_handler(128, irq_syscall);
}