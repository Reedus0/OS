#include "arch/x86_64/gdt/gdt.h"
#include "arch/x86_64/irq/idt/idt.h"
#include "arch/x86_64/irq/handlers.h"
#include "arch/x86_64/drivers/pic/pic.h"
#include "arch/x86_64/drivers/keyboard/keyboard.h"
#include "arch/x86_64/memory/discover.h"
#include "arch/x86_64/boot/multiboot2.h"
#include "arch/x86_64/drivers/bus/pci/pci.h"
#include "kernel/driver.h"

void init_hal(multiboot2_info_t* mbd) {
    
    init_gdt();

    init_idt();
    init_irq_handlers();

    driver_t pic_driver = init_pic_driver();
    register_driver(pic_driver);

    driver_t keyboard_driver = init_keyboard_driver();
    register_driver(keyboard_driver);

    driver_t pci_driver = init_pci_driver();
    register_driver(pci_driver);

    init_drivers();

    enable_irq();

    discover_memory(mbd);

    init_heap();
}