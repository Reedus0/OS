#pragma once

#include <stdint.h>
#include <stddef.h>

#define KEYBOARD_PORT 0x60

uint8_t key_map[256] = { NULL, 49,   50,   51,   52,   53,   54,   55,
                         56,   57,   48,   45,   61,   NULL, NULL, 113,
                         119,  101,  114,  116,  121,  117,  105,  111,
                         112,  91,   93,   NULL, NULL, 97,   115,  100,
                         102,  103,  104,  106,  107,  108,  59,   39,  
                         96,   NULL, 92,   122,  120,  99,   118,  98,  
                         110,  109,  44,   46,   47,   NULL, NULL, NULL,
                         32
                        };

void keyboard_process_key();