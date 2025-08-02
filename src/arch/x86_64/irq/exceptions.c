#include "handlers.h"
#include "include/panic.h"
#include "kernel/printk.h"
#include "memory/paging.h"
#include "asm/paging.h"

static char* exceptions[] = {
    "Division error",
    "Debug",
    "Non-maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "Bound Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack-Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "",
    "x87 Floating-Point Exception",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating-Point Exception",
    "Virtualization Exception",
    "Control Protection Exception",
    "",
    "",
    "",
    "",
    "",
    "",
    "Hypervisor Injection Exception",
    "VMM Communication Exception",
    "Security Exception",
    ""
};

static void print_exception_info(irq_data_t* irq_data) {
    printk(NONE, "error code:   0x%x\noriginal rip: 0x%16x\noriginal rsp: 0x%16x\nrflags: 0x%8x\n",
        irq_data->error_code, irq_data->rip, irq_data->rsp, irq_data->rflags);
}

interrupt_t irq_handle_exception(irq_data_t* irq_data) {
    printk(NONE, "\nException: %s!\n", exceptions[irq_data->interrupt_number]);
    print_exception_info(irq_data);
    panic("Got an exception!");
}

interrupt_t irq_page_fault(irq_data_t* irq_data) {
    uint64_t page_fault_address = get_page_fault_address();
    printk(NONE, "\nPage fault address: 0x%x\n", page_fault_address);

    size_t virtual_address = page_fault_address & 0xFFFFFFF00000;
    size_t physical_address = allocate_physical_address(virtual_address);

    map_page(physical_address, virtual_address, 0x82);
    flush_page();
}