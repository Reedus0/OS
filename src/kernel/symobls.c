#include "io.h"
#include "symbols.h"
#include "elf.h"
#include "include/kalloc.h"

void load_kernel_symbols(char* kernel_path) {
    struct elf_64* kernel = read_elf(kernel_path);

    printk(NONE, "%x", 1);

    kfree(kernel);
}