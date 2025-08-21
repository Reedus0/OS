#include "hal/gdt/gdt.h"
#include "hal/idt/idt.h"
#include "asm/asm.h"
#include "asm/io.h"
#include "irq/handlers.h"
#include "memory/discover.h"
#include "boot/multiboot2.h"
#include "kernel/io.h"
#include "drivers/pci/pci.h"
#include "drivers/pic/pic.h"
#include "drivers/keyboard/keyboard.h"
#include "drivers/rtc/cmos/cmos.h"
#include "drivers/pci/ahci.h"
#include "drivers/tty/tty.h"
#include "drivers/partition/partition.h"
#include "include/module.h"
#include "include/scheduler.h"
#include "memory/heap.h"
#include "include/dev.h"

void init_hal(multiboot2_info_t* mbd) {

    disable_irq();
    init_gdt();

    discover_memory(mbd);

    g_terminal = init_tty_dev();

    printk(INFO, "Initiating devices...\n");
    void* ptr = kalloc(PAGE_SIZE - 0x1000);
    void* ptr2 = kalloc(PAGE_SIZE - 0x1000);
    print_heap();
    kfree(ptr);
    print_heap();
    void* ptr3 = kalloc(PAGE_SIZE - 0x1000);
    print_heap();
    kfree(ptr2);
    print_heap();
    void* ptr4 = kalloc(PAGE_SIZE - 0x1000);
    print_heap();
    while (1);
    init_paging();

    init_io();
    init_idt();
    init_irq_handlers();


    g_pic = init_pic_dev(PIC_DEFAULT_PORT_1, PIC_DEFAULT_PORT_2);
    MODULE_FUNCTION(g_pic->driver, PIC_REMAP)(g_pic, 0x20, 0x28);

    g_keyboard = init_keyboard_dev(KEYBOARD_DEFAULT_PORT, KEYBOARD_STATUS_PORT);
    g_cmos = init_cmos_dev();

    init_pci();

    g_ahci = init_ahci_dev(AHCI_DEFAULT_VENDOR, AHCI_DEFAULT_DEVICE_ID);
    g_hdd = init_ahci_drive_dev(g_ahci, 0);

    g_partition = init_partition_dev(g_hdd, 1);

    printk(SUCCESS, "Initiated modules!\n");

    printk(SUCCESS, "Initiated HAL!\n");
}