#include "hal/gdt/gdt.h"
#include "hal/idt/idt.h"
#include "asm/asm.h"
#include "asm/io.h"
#include "irq/handlers.h"
#include "memory/discover.h"
#include "boot/multiboot2.h"
#include "kernel/printk.h"
#include "drivers/pci/pci.h"
#include "drivers/pic/pic.h"
#include "drivers/keyboard/keyboard.h"
#include "drivers/rtc/cmos/cmos.h"
#include "drivers/pci/ahci.h"
#include "drivers/tty/tty.h"
#include "include/module.h"
#include "include/scheduler.h"
#include "memory/heap.h"
#include "include/dev.h"

void init_hal(multiboot2_info_t* mbd) {

    disable_irq();

    init_gdt();

    init_idt();
    init_irq_handlers();

    discover_memory(mbd);
    init_heap();
    init_paging();

    g_terminal = init_tty_dev();

    printk(INFO, "Initiating modules...\n");

    g_pic = init_pic_dev(PIC_DEFAULT_PORT_1, PIC_DEFAULT_PORT_2);
    MODULE_FUNCTION(g_pic->driver, PIC_REMAP)(g_pic, 0x20, 0x28);

    init_pci();

    module_t* cmos_module = init_cmos_module();
    register_module(cmos_module);

    g_keyboard = init_keyboard_dev(KEYBOARD_DEFAULT_PORT, KEYBOARD_STATUS_PORT);

    module_t* ahci_module = init_ahci_module();
    register_module(ahci_module);
    g_hdd->driver = ahci_module;

    printk(SUCCESS, "Initiated modules!\n");

    printk(SUCCESS, "Initiated HAL!\n");
}