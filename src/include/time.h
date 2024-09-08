#pragma once

#include <stdint.h>
#include <stddef.h>

struct {
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t month;
    uint16_t year;
} g_time;