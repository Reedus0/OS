#include "elf.h"

#include "include/fs.h"
#include "include/kalloc.h"

static void elf_read_tables(elf64_t* elf) {
    struct elf_sheader_64* current_sheader = elf->sheaders;
    struct elf_sheader_64* shstr_sheader = elf->sheaders + elf->file_header->e_shstrndx;

    for (size_t i = 0; i < elf->file_header->e_shnum; i++) {
        if (current_sheader->sh_type == SHT_STRTAB && current_sheader != shstr_sheader) {
            char* strtab = kalloc(current_sheader->sh_size);

            vfs_seek(elf->file, current_sheader->sh_offset);
            vfs_read_file(elf->file, strtab, current_sheader->sh_size);

            elf->strtab = strtab;
        }
        if (current_sheader->sh_type == SHT_SYMTAB) {
            elf->symtab = current_sheader->sh_offset;
            elf->symbols_count = current_sheader->sh_size / current_sheader->sh_entsize;

            char* symtab = kalloc(current_sheader->sh_size);

            vfs_seek(elf->file, current_sheader->sh_offset);
            vfs_read_file(elf->file, symtab, current_sheader->sh_size);

            elf->symtab = symtab;
        }
        current_sheader++;
    }
}

static char* elf_read_string(elf64_t* elf, size_t offset) {
    char* string = kalloc(64);

    strncpy(string, elf->strtab + offset, 64);

    return string;
}

static elf64_symbol_t* elf_read_symbol(elf64_t* elf, size_t index) {
    elf64_symbol_t* symbol = kalloc(sizeof(elf64_symbol_t));

    struct elf_symtab_entry_64* symtab_entry = (elf->symtab + index * 24);

    symbol->name = elf_read_string(elf, symtab_entry->st_name);
    symbol->address = symtab_entry->st_value;

    return symbol;
}

static void destroy_elf_symbol(elf64_symbol_t* symbol) {
    kfree(symbol->name);
    kfree(symbol);
}

static void elf_read_symbols(elf64_t* elf) {
    rb_tree_t symbols = rb_init();

    for (size_t i = 0; i < elf->symbols_count; i++) {
        elf64_symbol_t* new_symbol = elf_read_symbol(elf, i);

        if (!symbols.root) {
            symbols.root = &new_symbol->node;
            continue;
        }

        rb_node_t** link = &symbols.root;
        rb_node_t* parent = NULL;

        while (*link) {
            parent = *link;
            elf64_symbol_t* entry = container_of(parent, elf64_symbol_t, node);

            if (new_symbol->address < entry->address) link = &parent->left;
            else link = &parent->right;
        }

        rb_insert(parent, &new_symbol->node, link);
        rb_balance(&symbols, &new_symbol->node);
    }

    elf->symbols = symbols;
}

elf64_symbol_t* elf_get_symbol(elf64_t* elf, size_t address) {
    rb_node_t* node = elf->symbols.root;
    elf64_symbol_t* candidate = NULL;

    while (node) {
        elf64_symbol_t* data = container_of(node, elf64_symbol_t, node);

        if (data->address == address) {
            return data;
        }
        else if (data->address < address) {
            candidate = data;
            node = node->right;
        }
        else {
            node = node->left;
        }
    }

    return candidate;
}

elf64_t* read_elf(char* path) {
    elf64_t* elf = kalloc(sizeof(elf64_t));
    file_t* file = vfs_open_file(path);

    elf->file = file;

    struct elf_file_header_64* file_header = kalloc(sizeof(struct elf_file_header_64));

    vfs_read_file(elf->file, file_header, 64);

    elf->file_header = file_header;

    if (elf->file_header->e_phnum) {
        struct elf_pheader_64* pheaders = kalloc(elf->file_header->e_phnum * sizeof(struct elf_pheader_64));

        vfs_seek(elf->file, elf->file_header->e_phoff);
        vfs_read_file(elf->file, pheaders, elf->file_header->e_phnum * sizeof(struct elf_pheader_64));

        elf->pheaders = pheaders;
    }

    if (elf->file_header->e_shnum) {
        struct elf_sheader_64* sheaders = kalloc(elf->file_header->e_shnum * sizeof(struct elf_sheader_64));

        vfs_seek(elf->file, elf->file_header->e_shoff);
        vfs_read_file(elf->file, sheaders, elf->file_header->e_shnum * sizeof(struct elf_sheader_64));

        elf->sheaders = sheaders;
    }

    elf_read_tables(elf);
    elf_read_symbols(elf);

    kfree(elf->strtab);
    kfree(elf->symtab);

    return elf;
}

void destroy_elf(elf64_t* elf) {
    kfree(elf->pheaders);
    kfree(elf->sheaders);

    rb_node_t* node = elf->symbols.root;

    while (node = rb_first(&elf->symbols)) {
        elf64_symbol_t* symbol = container_of(node, elf64_symbol_t, node);

        rb_remove(&elf->symbols.root, node);
        destroy_elf_symbol(symbol);
    }

    vfs_close_file(elf->file);

    kfree(elf);
}