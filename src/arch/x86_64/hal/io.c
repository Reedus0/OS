#include "io.h"
#include "include/kalloc.h"

void init_io() {
    g_io_buffer = kalloc(4);
}