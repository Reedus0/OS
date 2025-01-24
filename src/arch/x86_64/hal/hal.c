#include "hal/gdt/gdt.h"
#include "hal/idt/idt.h"
#include "asm/asm.h"
#include "asm/io.h"
#include "irq/handlers.h"
#include "memory/discover.h"
#include "boot/multiboot2.h"
#include "kernel/printk.h"
#include "drivers/bus/pci/pci.h"
#include "drivers/pic/pic.h"
#include "drivers/keyboard/keyboard.h"
#include "drivers/ata/ide.h"
#include "drivers/tty/tty.h"
#include "include/module.h"
#include "include/scheduler.h"
#include "include/dev.h"

void init_hal(multiboot2_info_t* mbd) {

    disable_irq();

    init_gdt();

    init_idt();
    init_irq_handlers();

    init_heap();

    module_t* tty_module = init_tty_module();
    register_module(tty_module);

    g_terminal.driver = tty_module;

    printk(INFO, "Initiating modules...\n");

    g_pic_module = init_pic_module();
    register_module(g_pic_module);
    MODULE_FUNCTION(g_pic_module, PIC_REMAP)(0x20, 0x28);

    g_pci_module = init_pci_module();
    register_module(g_pci_module);
    MODULE_FUNCTION(g_pci_module, PCI_CHECK_BUSES)();

    module_t* keyboard_module = init_keyboard_module();
    register_module(keyboard_module);

    g_keyboard.driver = keyboard_module;
    // module_t* cmos_module = init_cmos_module();
    // register_module(cmos_module);

    module_t* ide_module = init_ide_module();
    register_module(ide_module);
    MODULE_FUNCTION(ide_module, IDE_SET_PORT)(0x1F0, 1);

    g_hdd.driver = ide_module;

    printk(SUCCESS, "Initiated modules!\n");

    // discover_memory(mbd);

    printk(SUCCESS, "Initiated HAL!\n");
}