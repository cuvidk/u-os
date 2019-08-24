#include "string.h"

size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len] != 0) {
        len++;
    }
    return len;
}

void* memset(void* b, int c, size_t len) {
    unsigned char* dst = (unsigned char*)b;

    while (len--) {
        *dst = (unsigned char)c;
        ++dst;
    }

    return b;
}

