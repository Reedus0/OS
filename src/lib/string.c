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
    int result = 0;
    while (*string != '\0') {
        if (*string == character) {
            return string + result - 1;
        }
        result++;
        string++;
    }
    return 0;
}