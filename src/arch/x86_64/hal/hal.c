#include "arch/x86_64/gdt/gdt.h"
#include "arch/x86_64/irq/idt/idt.h"
#include "arch/x86_64/irq/handlers.h"
#include "arch/x86_64/memory/discover.h"
#include "arch/x86_64/boot/multiboot2.h"
#include "drivers/bus/pci/pci.h"
#include "drivers/pic/pic.h"
#include "drivers/keyboard/keyboard.h"
#include "include/module.h"

void init_hal(multiboot2_info_t* mbd) {

    init_gdt();

    init_idt();
    init_irq_handlers();

    init_heap();

    init_pic_module();
    register_module(g_pic_module);
    MODULE_FUNCTION(g_pic_module, PIC_REMAP)(0x20, 0x28);

    init_keyboard_module();
    register_module(g_keyboard_module);

    init_pci_module();
    register_module(g_pci_module);
    MODULE_FUNCTION(g_pci_module, PCI_CHECK_BUSES)();

    enable_irq();

    discover_memory(mbd);
}