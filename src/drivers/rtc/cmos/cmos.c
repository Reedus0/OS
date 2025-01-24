#include "include/time.h"
#include "include/kalloc.h"
#include "cmos.h"

static uint8_t cmos_read_register(uint8_t offset) {
    out8(CMOS_REGISTER_SELECT_PORT, (1 << 7) | (offset));
    io_wait();
    return in8(CMOS_DATA_READ_PORT);
}

static void cmos_read_time() {
    uint8_t second = cmos_read_register(SECONDS_REGISTER) % 60;
    uint8_t minute = cmos_read_register(MINUTES_REGISTER) % 60;
    uint8_t hour = cmos_read_register(HOURS_REGISTER) % 24;
    uint8_t day = cmos_read_register(DAY_REGISRER) % 31;
    uint8_t month = cmos_read_register(MONTH_REGISTER) % 12;
    uint16_t year = cmos_read_register(YEAR_REGISTER) % 100;
    year = year > 90 ? 1900 + year : 2000 + year;

    set_time(second, minute, hour, day, month, year);
}

static void init_cmos() {
    cmos_read_time();
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