#pragma once

#include <stdint.h>
#include <stddef.h>

#include "include/module.h"

#define PIIX_DEFAULT_BASE_ADDRESS 0xE041

#define PIIX_VENDOR 0
#define PIIX_DEVICE 2
#define PIIX_COMMAND 4
#define PIIX_STATUS 6
#define PIIX_REVISION 8
#define PIIX_CLASS_CODE 9
#define PIIX_LATENCE_TIMER 13
#define PIIX_HEADER_TYPE 14
#define PIIX_IDE_BASE_ADDRESS 32
#define PIIX_IDE_TIMING 64

module_t g_piix_module;
module_t init_piix_module();