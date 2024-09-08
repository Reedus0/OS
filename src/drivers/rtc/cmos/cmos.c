#include "cmos.h"

static uint8_t cmos_read_register(uint8_t offset) {
    out8(CMOS_REGISTER_SELECT_PORT, (1 << 7) | (offset));
    io_wait();
    return in8(CMOS_DATA_READ_PORT);
}

static void init_cmos() {

}

static void deinit_cmos() {
    
}

module_t* init_cmos_module() {
    module_t* cmos_module = kalloc(sizeof(module_t));

    cmos_module->name = "CMOS module";

    cmos_module->init = init_cmos;

    cmos_module->deinit = deinit_cmos;

    return cmos_module;
}