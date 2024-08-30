#include "entry.h"
#include "fat.h"
#include "lib/string.h"

char* fat_entry_get_lfn(fat_entry_t* fat_entry) {
    char* name = kalloc(64);

    while (fat_entry->attributes == LFN) {
        fat_long_name_t* fat_long_name = fat_entry;
        if (fat_long_name->order & 0x40 == 0x40) {
            fat_long_name->order ^= 0x40;
        }
        size_t position = (fat_long_name->order - 1) * 13; 
        char* ptr = fat_long_name->name_1;
        for (size_t i = 0; i < 13; i++) {
            if (i >= 5) {
                ptr = fat_long_name->name_2;
                name[position + i] = *(ptr + (i * 2) - 10);
                continue;
            }
            if (i >= 11) {
                ptr = fat_long_name->name_2;
                name[position + i] = *(ptr + (i * 2) - 22);
                continue;
            }
            name[position + i] = *(ptr + i * 2);
        }
        fat_entry++;
    }
    return name;
}