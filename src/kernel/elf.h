#pragma once

#include <stdint.h>
#include <stddef.h>

#include "include/fs.h"
#include "include/types.h"
#include "include/rb.h"
#include "include/macro.h"

#define SHT_SYMTAB 2
#define SHT_STRTAB 3

struct elf_pheader_64 {
    uint32_t p_type;
    uint32_t p_flags;
    uint64_t p_offset;
    uint64_t p_vaddr;
    uint64_t p_paddr;
    uint64_t p_filesz;
    uint64_t p_memsz;
    uint64_t p_align;
} __attribute__((packed));

struct elf_sheader_64 {
    uint32_t sh_name;
    uint32_t sh_type;
    uint64_t sh_flags;
    uint64_t sh_addr;
    uint64_t sh_offset;
    uint64_t sh_size;
    uint32_t sh_link;
    uint32_t sh_info;
    uint64_t sh_addralign;
    uint64_t sh_entsize;
} __attribute__((packed));

struct elf_file_header_64 {
    uint8_t e_ident[16];
    uint16_t e_type;
    uint16_t e_machine;
    uint32_t e_version;
    uint64_t e_entry;
    uint64_t e_phoff;
    uint64_t e_shoff;
    uint32_t e_flags;
    uint16_t e_ehsize;
    uint16_t e_phentsize;
    uint16_t e_phnum;
    uint16_t e_shentsize;
    uint16_t e_shnum;
    uint16_t e_shstrndx;
} __attribute__((packed));

struct elf_symtab_entry_64 {
    uint32_t st_name;
    uint8_t st_info;
    uint8_t st_other;
    uint16_t st_shndx;
    uint64_t st_value;
    uint64_t st_size;
} __attribute__((packed));

struct elf64_symbol {
    uint64_t address;
    rb_node_t node;
    char* name;
};
typedef struct elf64_symbol elf64_symbol_t;

struct elf64 {
    struct elf_file_header_64* file_header;
    struct elf_pheader_64* pheaders;
    struct elf_sheader_64* sheaders;

    file_t* file;

    size_t strtab;
    size_t symtab;

    rb_tree_t symbols;
    size_t symbols_count;
};
typedef struct elf64 elf64_t;


elf64_t* read_elf(char* path);
elf64_symbol_t* elf_get_symbol(elf64_t* elf, size_t address);
void destroy_elf(elf64_t* elf);

elf64_t* g_kernel_elf;