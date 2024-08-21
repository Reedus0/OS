#include "gdt/gdt.h"
#include "irq/idt/idt.h"
#include "irq/handlers.h"
#include "memory/discover.h"
#include "boot/multiboot2.h"
#include "drivers/bus/pci/pci.h"
#include "drivers/pic/pic.h"
#include "drivers/keyboard/keyboard.h"
#include "drivers/ata/ide.h"
#include "include/module.h"

void init_hal(multiboot2_info_t* mbd) {

    byte buf[1024];

    init_gdt();

    init_idt();
    init_irq_handlers();

    init_heap();

    init_pic_module();
    register_module(&g_pic_module);
    MODULE_FUNCTION(g_pic_module, PIC_REMAP)(0x20, 0x28);

    init_keyboard_module();
    register_module(&g_keyboard_module);

    init_pci_module();
    register_module(&g_pci_module);
    MODULE_FUNCTION(g_pci_module, PCI_CHECK_BUSES)();

    init_ide_module();
    register_module(&g_ide_module);
    MODULE_FUNCTION(g_ide_module, IDE_READ_SECTORS_SLAVE)(buf, 1, 0);

    for (size_t j = 0; j < 512; j++) {

        printk("%x", buf[j]);
    }

    enable_irq();

    discover_memory(mbd);
}