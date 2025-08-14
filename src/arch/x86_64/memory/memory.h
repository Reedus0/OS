#pragma once

#include <stdint.h>
#include <stddef.h>

#include "shell/shell.h"

#define HEAP_PHYSICAL_ADDRESS_OFFSET 0x10000000
#define PAGE_SIZE 0x200000
#define MAX_ADDRESS_SIZE 48
#define MAX_ADDRESS_MASK 0xFFFFFFFFFF

/*
Reference: https://wiki.osdev.org/Memory_Map_(x86)

Physical
#-----------------------#-----------------------------#----------------------------#----------------------------#
   0x1000000-0xfffffff     0x10000000-0xefffffff          0xf0000000-0xffffffff       0x100000000-0xffffffffffff
         kernel                     heap                       hardware                        heap
#-----------------------#-----------------------------#-----------------------------#-----------------------------#
Virtual
#------------------------#------------------------------------#
    0x0-0x7fffffffffff     0xffff800000000000-0xffffffffffff
          heap                         kernel
#------------------------#------------------------------------#
*/

shell_command sh_memory(char* command);