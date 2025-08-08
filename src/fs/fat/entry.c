#include "entry.h"
#include "table.h"
#include "fs.h"
#include "lib/string.h"
#include "include/time.h"

char* fat_entry_read_lfn(fat_entry_t* fat_entry) {
    char* name = kalloc(64);

    while (fat_entry->attributes == LFN) {
        fat_long_name_t* fat_long_name = fat_entry;
        char order = fat_long_name->order;
        if ((order & 0x40) == 0x40) {
            order ^= 0x40;
        }
        size_t position = (order - 1) * 13;
        size_t offset = 0;
        char* ptr = fat_long_name->name_1;
        for (size_t i = 0; i < 13; i++) {
            if (i >= 5) {
                ptr = fat_long_name->name_2;
                offset = 10;
            }
            if (i >= 11) {
                ptr = fat_long_name->name_3;
                offset = 22;
            }
            name[position + i] = ptr[i * 2 - offset];
        }
        fat_entry++;
    }
    return name;
}

static byte get_checksum(char* name) {
    byte result = 0;
    for (size_t i = 0; i < 12; i++) {
        if (strlen(name) <= i) {
            result += 0x20;
            continue;
        }
        result += toupper(name[i]);
    }
    return result;
}

static fat_long_name_t* create_lfn(char* name) {
    size_t name_length = strlen(name);
    size_t entry_count = (name_length / 13) + 1;
    byte checksum = get_checksum(name);
    fat_long_name_t* result = kalloc(entry_count * sizeof(fat_long_name_t));

    for (size_t i = 0; i < entry_count; i++) {
        result[i].order = i + 1;
        result[i].attribute = LFN;
        result[i].checksum = checksum;
        result[i].reserved_1 = 0;

        char* ptr = result->name_1;
        size_t offset = 0;

        for (size_t j = 0; j < 26; j += 2) {
            bool name_not_ended = name_length + 1 > (j / 2) * (i + 1);
            if (j >= 10) {
                ptr = result->name_2;
                offset = 10;
            }
            if (j >= 22) {
                ptr = result->name_3;
                offset = 22;
            }
            if (name_not_ended) {
                ptr[j - offset] = name[(j / 2) * (i + 1)];
            }
            else {
                ptr[j - offset] = 0xFF;
                ptr[j - offset + 1] = 0xFF;
            }
        }
    }
    result[entry_count - 1].order |= 0x40;

    return result;
}

fat_entry_t* fat_entry_create(char* name, size_t cluster, enum FAT_ATTRIBUTES attributes) {
    size_t entry_count = ((strlen(name) / 13) + 2);
    fat_entry_t* result = kalloc(entry_count * sizeof(fat_entry_t));

    fat_entry_t* file_name = create_lfn(name);
    for (size_t i = 0; i < entry_count - 1; i++) {
        fat_entry_add(result + i, file_name + i);
    }
    kfree(file_name);

    fat_entry_t* last_entry = (result + entry_count - 1);

    for (size_t i = 0; i < 12; i++) {
        if (strlen(name) <= i) {
            last_entry->name[i] = ' ';
            continue;
        }
        last_entry->name[i] = toupper(name[i]);
    }

    struct time ktime = time();

    last_entry->creation_time = ktime.hour | (uint16_t)(ktime.minute) >> 5 | (uint16_t)(ktime.second * 2) >> 11;
    last_entry->creation_date = ktime.year | (uint16_t)(ktime.month) >> 7 | (uint16_t)(ktime.day) >> 11;

    last_entry->attributes = attributes;
    last_entry->cluster_low = cluster & 0xFFFF;
    last_entry->cluster_high = (cluster >> 16) & 0xFFFF;

    return result;
}

void fat_entry_add(fat_entry_t* dir, fat_entry_t* fat_entry) {
    while (VALID_ENTRY(dir)) dir++;
    memcpy(dir, fat_entry, sizeof(fat_entry_t));
}

void fat_entry_remove(fat_entry_t* dir, size_t index) {
    for (size_t i = 0; i < index + 1; i++) {
        dir++;
    }
    while (VALID_ENTRY(dir)) {
        *(dir - 1) = *dir;
        dir++;
    }
    memset(dir - 1, 0, sizeof(fat_entry_t));
}