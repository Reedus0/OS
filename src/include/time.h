#pragma once

#include <stdint.h>
#include <stddef.h>

static struct time {
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t month;
    uint16_t year;
} ktime;

void set_time(uint8_t second, uint8_t minute, uint8_t hour, uint8_t day, uint8_t month, uint16_t year) {
    ktime.second = second;
    ktime.minute = minute;
    ktime.hour = hour;
    ktime.day = day;
    ktime.month = month;
    ktime.year = year;
}

void update_time() {
    ktime.second += 1;
    if (ktime.second == 60) {
        ktime.second = 0;
        ktime.minute += 1;
    }
    if (ktime.minute == 60) {
        ktime.minute = 0;
        ktime.hour += 1;
    }
    if (ktime.hour == 24) {
        ktime.hour = 0;
        ktime.day += 1;
    }
}

struct time time() {
    return ktime;
}