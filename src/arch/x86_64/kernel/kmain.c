#include "hal/hal.h"
#include "memory/memory.h"
#include "drivers/pci/pci.h"
#include "shell/utils.h"
#include "shell/shell.h"
#include "include/task.h"
#include "include/scheduler.h"
#include "fs/vfs.h"
#include "asm/io.h"
#include "include/asm.h"
#include "kernel/elf.h"

void kmain() {
    init_hal();
    init_vfs();
    init_scheduler();

    g_kernel_elf = read_elf("/kernel/kernel.bin");

    enable_irq();

    static shell_function_t functions[] = {
        {NULL, NULL},
        {"hello", sh_hello},
        {"clear", sh_clear},
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
        {"r ", sh_readelf},
        {NULL, NULL},
    };

    schedule_task(create_task(init_shell, functions));
    while (1);
}