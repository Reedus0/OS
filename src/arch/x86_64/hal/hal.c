#include "arch/x86_64/gdt/gdt.h"
#include "arch/x86_64/irq/idt/idt.h"
#include "arch/x86_64/irq/handlers.h"
#include "arch/x86_64/drivers/driver.h"
#include "arch/x86_64/drivers/pic/pic.h"
#include "arch/x86_64/drivers/keyboard/keyboard.h"

void init_hal() {
    init_gdt();

    init_idt();
    init_irq_handlers();

    struct driver pic_driver = init_pic_driver();
    register_driver(pic_driver);

    struct driver keyboard_driver = init_keyboard_driver();
    register_driver(keyboard_driver);

    init_drivers();

    enable_irq();
    print_string("IRQ OK\n");
}