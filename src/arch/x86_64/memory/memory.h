#pragma once

#include <stdint.h>
#include <stddef.h>

#include "shell/shell.h"

#define HEAP_PHYSICAL_ADDRESS_OFFSET 0x10000000
#define USUBABLE_MEMORY 0x1000000
#define PAGE_SIZE 0x200000

/*
Reference: https://wiki.osdev.org/Memory_Map_(x86)

Physical
#-----------------------#-----------------------#-----------------------------#----------------------------#------------------------------#
       0x0-0xffffff        0x1000000-0xfffffff       0x10000000-0xefffffff        0xf0000000-0xffffffff       0x100000000-0xffffffffffff
         hardware               kernel                     heap                       hardware                        heap
#-----------------------#-----------------------#-----------------------------#----------------------------#------------------------------#
Virtual
#------------------------#----------------------------------------#----------------------------------------#----------------------------------------#
    0x0-0x7fffffffffff     0xffff800001000000-0xffff80000fffffff     0xffff800010000000-0xffffefffffffffff    0xfffff00000000000-0xffffffffffff
          heap                          kernel                                   kernel_heap                             physmap
#------------------------#----------------------------------------#----------------------------------------#----------------------------------------#
*/

shell_command sh_memory(char* command);