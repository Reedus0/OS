#include "entry.h"
#include "table.h"
#include "fs.h"
#include "lib/string.h"

char* fat_entry_read_lfn(fat_entry_t* fat_entry) {
    char* name = kalloc(64);

    while (fat_entry->attributes == LFN) {
        fat_long_name_t* fat_long_name = fat_entry;
        char order = fat_long_name->order;
        if ((order & 0x40) == 0x40) {
            order ^= 0x40;
        }
        size_t position = (order - 1) * 13; 
        char* ptr = fat_long_name->name_1;
        for (size_t i = 0; i < 13; i++) {
            if (i >= 5) {
                ptr = fat_long_name->name_2;
                name[position + i] = ptr[i * 2 - 10];
                continue;
            }
            if (i >= 11) {
                ptr = fat_long_name->name_2;
                name[position + i] = ptr[i * 2 - 22];
                continue;
            }
            name[position + i] = ptr[i * 2];
        }
        fat_entry++;
    }
    return name;
}

static byte get_checksum(char* name, size_t offset) {
    byte result = 0;
    for (size_t i = 0; i < 13; i++) {
        result += name[i + offset * 13];
    }
    return result;
}

static fat_long_name_t* create_lfn(char* name) {
    size_t entry_count = ((strlen(name) / 13) + 1);
    fat_long_name_t* result = kalloc(entry_count * sizeof(fat_long_name_t));

    for (size_t i = 0; i < entry_count; i++) {
        result[i].order = i + 1;
        result[i].attribute = LFN;
        result[i].entry_type = 1;
        result[i].checksum = get_checksum(name, i);
        result[i].reserved_1 = 0;

        for (size_t j = 0; j < 13; j++) {
            if (j >= 5) {
                result[i].name_2[j * 2 - 10] = name[j + i * 13];
                continue;
            }
            if (j >= 11) {
                result[i].name_3[j * 2 - 22] = name[j + i * 13];
                continue;
            }
            result[i].name_1[j * 2] = name[j + i * 13];
        }
    }
    result[entry_count - 1].order |= 0x40;

    return result;
}

fat_entry_t* fat_entry_create(char* name, enum FAT_ATTRIBUTES attributes) {
    size_t entry_count = ((strlen(name) / 13) + 2);
    fat_entry_t* result = kalloc(entry_count * sizeof(fat_entry_t));

    fat_entry_t* file_name = create_lfn(name);
    for (size_t i = 0; i < entry_count - 1; i++) {
        result[i] = file_name[i];
    }
    kfree(file_name);
    
    for (size_t i = 0; i < 13; i++) {
        if (i > strlen(name)) {
            result[entry_count].name[i] = ' ';
            continue;
        }
        result[entry_count].name[i] = toupper(name[i]);
    }

    result->attributes = attributes;

    return result;
}

void fat_entry_add(fat_entry_t* dir, fat_entry_t* fat_entry) {
    while(VALID_ENTRY(dir)) dir++;
    *dir = *fat_entry;
}

void fat_entry_remove(fat_entry_t* dir, size_t index) {
    for (size_t i = 0; i < index + 1; i++){
        dir++;
    }
    while(VALID_ENTRY(dir)) {
        *(dir - 1) = *dir;
        dir++;
    }
    memset(dir - 1, 0, sizeof(fat_entry_t));
}