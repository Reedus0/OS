#include "elf.h"

#include "include/fs.h"
#include "include/kalloc.h"
#include "fs/vfs.h"

struct elf_64* read_elf(char* path) {
    struct elf_64* elf = kalloc(sizeof(struct elf_64));
    file_t* file = vfs_open_file(path);

    struct elf_file_header_64* file_header = kalloc(sizeof(struct elf_file_header_64));

    vfs_read_file(file, file_header, 64);

    elf->file_header = file_header;

    if (elf->file_header->e_phnum) {
        struct elf_pheader_64* pheaders = kalloc(elf->file_header->e_phnum * sizeof(struct elf_pheader_64));

        vfs_seek(file, elf->file_header->e_phoff);
        vfs_read_file(file, pheaders, elf->file_header->e_phnum * sizeof(struct elf_pheader_64));

        elf->pheaders = pheaders;
    }

    if (elf->file_header->e_shnum) {
        struct elf_sheader_64* sheaders = kalloc(elf->file_header->e_shnum * sizeof(struct elf_sheader_64));

        vfs_seek(file, elf->file_header->e_shoff);
        vfs_read_file(file, sheaders, elf->file_header->e_shnum * sizeof(struct elf_sheader_64));

        elf->sheaders = sheaders;
    }

    vfs_close_file(file);

    return elf;
}