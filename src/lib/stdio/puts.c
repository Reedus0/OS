#include "include/print.h"
#include "puts.h"

int puts(const char* str) {
    print_string(str);
    print_newline();
    return 1;
}