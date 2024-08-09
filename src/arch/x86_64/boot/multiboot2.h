#pragma once

// Reference: https://www.gnu.org/software/grub/manual/multiboot2/multiboot.html

#include <stdint.h>
#include <stddef.h>

#define TAG_MMAP 0x6
#define TAG_BASE_ADDRESS 0x15

struct multiboot2_tag {
    uint32_t type;
    uint32_t size;
};
typedef struct multiboot2_tag multiboot2_tag_t;

struct multiboot2_info {    
    uint32_t size;
    uint32_t reserved;
    struct multiboot2_tag tags;
};
typedef struct multiboot2_info multiboot2_info_t;

struct multiboot2_tag_base_address {
    uint32_t type;
    uint32_t size;
    uint32_t base_address;
};
typedef struct multiboot2_tag_base_address multiboot2_tag_base_address_t;

struct multiboot2_mmap_entry {
  uint64_t address;
  uint64_t length;
  uint32_t type;
  uint32_t zero;
};
typedef struct multiboot2_mmap_entry multiboot2_memory_map_t;

struct multiboot2_tag_mmap {
  uint32_t type;
  uint32_t size;
  uint32_t entry_size;
  uint32_t entry_version;
  struct multiboot2_mmap_entry entries;  
};
typedef struct multiboot2_tag_mmap multiboot2_tag_mmap_t;