#include "boot/multiboot2.h"
#include "hal/hal.h"
#include "memory/memory.h"
#include "drivers/bus/pci/pci.h"
#include "shell/utils.h"
#include "shell/shell.h"
#include "include/task.h"
#include "include/scheduler.h"
#include "fs/vfs.h"
#include "asm/io.h"

void test_task() {
    printk(NONE, "TASK OK");
    exit_task();
}

void kmain(multiboot2_info_t* mbd) {
    init_hal(mbd);

    printk(INFO, "Initiating scheduler...\n");

    init_scheduler();

    printk(SUCCESS, "Initiated scheduler!\n");

    enable_irq();

    static shell_function_t functions[] = {
        {NULL, NULL},
        {"hello", sh_hello},
        {"clear", sh_clear},
        {"module", sh_module},
        {"pci", sh_pci},
        {"memory", sh_memory},
        {"cd ", sh_cd},
        {"ls", sh_ls},
        {"mkfile ", sh_mkfile},
        {"mkdir ", sh_mkdir},
        {"rm ", sh_rm},
        {"rmdir", sh_rmdir},
        {"read ", sh_read},
        {"write ", sh_write},
        {"time", sh_time},
        {"task", sh_task},
        {NULL, NULL},
    };

    init_vfs();
    task_t* shell_task = create_task(init_shell);
    schedule_task(shell_task);
    while(1);
}