#pragma once

// Reference: https://wiki.osdev.org/Detecting_Memory_(x86)

#include <stdint.h>
#include <stddef.h>

#include "arch/x86_64/boot/multiboot2.h"

size_t g_available_memory;

void discover_memory(multiboot2_info_t* mbd);