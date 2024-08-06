#include "arch/x86_64/io.h"
#include "include/print.h"

void kmain () {
    char* string = "Hello, Kernel!\n";
    print_clear();
    print_string(string);

    setup_gdt();
    setup_idt();
    while(1); 
}