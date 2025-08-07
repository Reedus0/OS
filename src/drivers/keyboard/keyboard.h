#pragma once

#include <stdint.h>
#include <stddef.h>

#include "include/module.h"
#include "include/dev.h"

#define KEYBOARD_DEFAULT_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

#define BUFFER_EMPTY 0
#define BUFFER_FULL 1

uint8_t g_key_map[256] = {
    NULL, 49,   50,   51,   52,   53,   54,   55,
    56,   57,   48,   45,   61,   8,    NULL, 113,
    119,  101,  114,  116,  121,  117,  105,  111,
    112,  91,   93,   10,   NULL, 97,   115,  100,
    102,  103,  104,  106,  107,  108,  59,   39,
    96,   NULL, 92,   122,  120,  99,   118,  98,
    110,  109,  44,   46,   47,   NULL, NULL, NULL,
    32, NULL,
};

uint8_t g_caps_map[256] = {
    NULL, 33,   64,   35,   36,   37,   94,   38,
    42,   40,   41,   95,   43,   8,    NULL, 81,
    87,   69,   82,   84,   89,   85,   73,   79,
    80,   123,  125,  10,   NULL, 65,   83,   68,
    70,   71,   72,   74,   75,   76,   58,   34,
    64,   NULL, 124,  90,   88,   67,   86,   66,
    78,   77,   60,   62,   63,   NULL, NULL, NULL,
    32, NULL,
};

#define KEYBOARD_SET_PORT 16
#define KEYBOARD_SET_STATUS_PORT 17

struct keyboard_data {
    uint16_t port;
    uint16_t status;
    uint8_t caps_mode;
};
typedef struct keyboard_data keyboard_data_t;

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end);
void update_cursor(int x, int y);

module_t* init_keyboard_module();
dev_t* init_keyboard_dev(uint16_t port, uint16_t status);
void deinit_keyboard_dev(dev_t* keyboard_dev);