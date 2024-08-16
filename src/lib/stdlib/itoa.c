#include "itoa.h"

char *itoa(int num, char *str, int radix) {
    int i = 0;

    if (num == 0) {
        return "0";
    }

    while (num > 0) {
        size_t current_number = num % radix;
        if(current_number > 9) {
            str[i] = (current_number % 10) + 0x60 + 1;
        } else {
            str[i] = current_number + 0x30;
        }
        num /= radix;
        i++;
    }

    str[i] = '\0';

    for (int j = 0; j < i / 2; j++) {
        char tmp = str[j];
        str[j] = str[i - j - 1];
        str[i - j - 1] = tmp;
    }

    return str;
}