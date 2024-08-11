#include "handlers.h"
#include "include/panic.h"
#include "arch/x86_64/memory/paging.h"
#include "arch/x86_64/asm/paging.h"

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
    printf("error code: 0x%x\noriginal rip: 0x%x\nrflags: 0x%x\noriginal rsp: 0x%x\n", 
     irq_data->error_code, irq_data->original_rip, irq_data->rflags, irq_data->original_rsp);
    printf("rsp: 0x%x\nrbp: 0x%x\nrsi: 0x%x\nrdi: 0x%x\n",
     irq_data->regs.rsp, irq_data->regs.rbp,irq_data->regs.rsi, irq_data->regs.rdi);
    printf("rdx: 0x%x\nrcx: 0x%x\nrbx: 0x%x\nrax: 0x%x\n",
     irq_data->regs.rdx, irq_data->regs.rcx, irq_data->regs.rbx, irq_data->regs.rax);
}

interrupt irq_handle_exception(irq_data_t* irq_data) {
    printf("Exception: %s!\n", exceptions[irq_data->interrupt_number]);
    print_exception_info(irq_data);
    panic("Got an exception!");
}

interrupt irq_page_fault(irq_data_t* irq_data) {
    uint64_t page_fault_address = get_page_fault_address();
    printf("Page fault address: 0x%x\n", page_fault_address);
    size_t page = get_page_index(page_fault_address);
    map_page(page);
    flush_page();
}