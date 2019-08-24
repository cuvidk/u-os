#include "xform.h"
#include "string.h"
#include <stdint.h>

static const char* digits = "0123456789";
static const char* hex_digits = "0123456789abcdef";

void itoa(int64_t x, char* out) {
    if (!out)
        return;

    int64_t x_aux = x;

    char y[MAX_INT64_DIGITS];
    memset(y, 0, sizeof(y));
    memset(out, 0, sizeof(y));

    out[0] = digits[0];
    if (x == 0)
        return;

    short i = 0;
    while (x_aux) {
        char c = digits[ABS(x_aux % 10)];
        y[i++] = c;
        x_aux /= 10;
    }

    short j = 0;
    if (x < 0)
        out[j++] = '-';

    --i;

    while (i >= 0)
        out[j++] = y[i--];
}

void utoa(uint64_t x, char* out) {
    if (!out)
        return;

    uint64_t x_aux = x;

    char y[MAX_UINT64_DIGITS];
    memset(y, 0, sizeof(y));
    memset(out, 0, sizeof(y));

    out[0] = digits[0];
    if (x == 0)
        return;

    short i = 0;
    while (x_aux) {
        char c = digits[x_aux % 10];
        y[i++] = c;
        x_aux /= 10;
    }

    --i;

    short j = 0;
    while (i >= 0)
        out[j++] = y[i--];
}

void hex(uint64_t x, char* out) {
    if (!out)
        return;

    char y[MAX_HEX_DIGITS];
    memset(y, 0, sizeof(y));
    memset(out, 0, sizeof(y));

    out[0] = '0';
    out[1] = 'x';
    out[2] = hex_digits[0];
    if (x == 0)
        return;

    short i = 0;
    while (x) {
        char c = hex_digits[x % 16];
        y[i++] = c;
        x /= 16;
    }

    --i;

    short j = 2;
    while (i >= 0)
        out[j++] = y[i--];
}
