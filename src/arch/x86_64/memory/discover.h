#pragma once

#include <stdint.h>
#include <stddef.h>

// Reference: https://wiki.osdev.org/Detecting_Memory_(x86)

#include "include/types.h"
#include "boot/multiboot2.h"
#include "include/macro.h"

#define USUBABLE_MEMORY 0x1000000

struct memory_chunk {
    uint64_t start;
    uint64_t end;
};
typedef struct memory_chunk memory_chunk_t;

/*
Physical
#-----------------------#-----------------------#-----------------------------#----------------------------#------------------------------#
       0x0-0xffffff        0x1000000-0xfffffff       0x10000000-0xefffffff        0xf0000000-0xffffffff       0x100000000-0xffffffffffff
         hardware               kernel                     heap                       hardware                        heap
#-----------------------#-----------------------#-----------------------------#----------------------------#------------------------------#
Virtual
#---------------------------#------------------------------------------#------------------------------------------#----------------------------------------#
  0x10000000-0x7fffffffffff     0xffff800001000000-0xffff80000fffffff     0xffff800010000000-0xffffefffffffffff        0xfffff00000000000-0xffffffffffff
          heap                                kernel                                   kernel_heap                                physmap
#---------------------------#------------------------------------------#------------------------------------------#----------------------------------------#
*/

void discover_memory(multiboot2_info_t* mbd);