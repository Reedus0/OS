#include "include/print.h"

void kmain () {
    char* string = "Hello, Kernel!\n";
    print_clear();
    print_string(string);

    setup_gdt();
    while(1); 
}