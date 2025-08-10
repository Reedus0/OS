#include "string.h"

int strlen(const char* string) {
    int result = 0;
    while (*string != '\0') {
        result++;
        string++;
    }
    return result;
}

bool strncmp(const char* first, const char* second, int n) {
    for (size_t i = 0; i < n; i++) {
        if (first[i] != second[i]) return 0;
    }

    return 1;
}

bool strcmp(const char* first, const char* second) {
    int first_length = strlen(first);
    int second_length = strlen(second);

    if (first_length != second_length) return 0;

    return strncmp(first, second, first_length);
}

char* strchr(const char* string, char character) {
    while (*string != '\0') {
        if (*string == character) {
            return string;
        }
        string++;
    }
    return 0;
}

char* strrchr(const char* string, char character) {
    char* result = 0;
    while (*string != '\0') {
        if (*string == character) {
            result = string;
        }
        string++;
    }
    return result;
}

int strncpy(char* dest, const char* src, int n) {
    int result = 0;
    for (size_t i = 0; i < n; i++) {
        dest[i] = src[i];
        result++;
    }
    return result;
}

char tolower(char character) {
    if (character > 0x40 && character < 0x5b) {
        return character + 0x20;
    }
    return character;
}

char toupper(char character) {
    if (character > 0x60 && character < 0x7b) {
        return character - 0x20;
    }
    return character;
}

char* ltrim(char* string) {
    char* char_pointer = string;
    while (*char_pointer == 0x20) char_pointer++;
    memcpy(string, char_pointer, strlen(char_pointer) + 1);
    return string;
}

char* rtrim(char* string) {
    char* back = string + strlen(string);
    while (*--back == 0x20);
    *(back + 1) = '\0';
    return string;
}

char* trim_string(char* string) {
    char* left_trim = ltrim(string);
    if (strlen(left_trim) == 0) {
        return left_trim;
    }
    return rtrim(left_trim);
}

void* memset(byte* dest, byte c, int count) {
    for (size_t i = 0; i < count; i++) {
        dest[i] = c;
    }
    return dest;
}

void* memcpy(byte* dest, byte* src, int count) {
    for (size_t i = 0; i < count; i++) {
        *(dest + i) = *(src + i);
    }
    return dest;
}

int isdigit(char character) {
    return character > 0x2F && character < 0x3A ? 1 : 0;
}

char* itoa(size_t num, char* str, int radix) {
    size_t i = 0;

    if (num == 0) {
        *str = '0';
        *(str + 1) = '\0';
        return str;
    }

    while (num > 0) {
        size_t current_number = num % radix;
        if (current_number > 9) {
            *(str + i) = (current_number % 10) + 0x60 + 1;
        }
        else {
            *(str + i) = current_number + 0x30;
        }
        num /= radix;
        i++;
    }

    *(str + i) = '\0';

    for (size_t j = 0; j < i / 2; j++) {
        char tmp = *(str + j);
        *(str + j) = *(str + i - j - 1);
        *(str + i - j - 1) = tmp;
    }

    return str;
}

int atoi(const char* str) {
    char buffer[64];
    if (!isdigit(*str)) {
        return 0;
    }

    int result = 0;

    while (isdigit(*str)) {
        result = result * 10 + *str - 0x30;
        str++;
    }

    return result;
}