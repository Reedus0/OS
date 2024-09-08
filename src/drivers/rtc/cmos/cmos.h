#pragma once

#include <stdint.h>
#include <stddef.h>

#include "include/module.h"

#define CMOS_REGISTER_SELECT_PORT 0x70
#define CMOS_DATA_READ_PORT 0x71

#define SECONDS_REGISTER 0
#define MINUTES_REGISTER 2
#define HOURS_REGISTER 4
#define DAY_REGISRER 6
#define MONTH_REGISTER 8
#define YEAR_REGISTER 9
#define CENTURY_REGISTER 50

module_t* init_cmos_module();