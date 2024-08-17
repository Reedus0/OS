#pragma once

#include <stdint.h>
#include <stddef.h>

#include "kernel/shell.h"

#define KERNEL_OFFSET 0x10000000
#define PAGE_SIZE 0x200000
#define MAX_ADDRESS_SIZE 40

shell_command sh_memory(char* command);