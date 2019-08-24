#include "vga.h"
#include "xform.h"
#include <stdint.h>
#include <stdarg.h>

#define PRINT_STR(str)                           \
   vga_print_range(needle, format - needle - 1); \
   vga_print(str);

#define PRINT_NUM(cast_t, spec_t, func_name)     \
   func_name((cast_t)va_arg(args, spec_t), num); \
   PRINT_STR(num)                                \
   ++format;


enum {
    SPEC_INT = 0x0,
    SPEC_UINT = 0x4,
    SPEC_HEX = 0x8
};

enum {
    SPEC_SZ8,
    SPEC_SZ16,
    SPEC_SZ32,
    SPEC_SZ64
};

typedef enum {
    SPEC_INT8 = SPEC_INT | SPEC_SZ8,
    SPEC_INT16,
    SPEC_INT32,
    SPEC_INT64,

    SPEC_UINT8 = SPEC_UINT | SPEC_SZ8,
    SPEC_UINT16,
    SPEC_UINT32,
    SPEC_UINT64,

    SPEC_HEX8 = SPEC_HEX | SPEC_SZ8,
    SPEC_HEX16,
    SPEC_HEX32,
    SPEC_HEX64,

    SPEC_STR,
    SPEC_ESCAPE,
    SPEC_END,
    SPEC_UNKN
} specifier_t;


// byte, word, double word, quad word
// %ib int8_t; %iw = int16_t; %id = int32_t; %iq = int64_t
// %xd uint32_t as hex, %xq uint64_t as hex
static specifier_t specifier_type(const char* format) {
    if (!format)
        return SPEC_UNKN;

    char _1st = format[0];
    char _2nd = format[1];
    specifier_t spec = SPEC_UNKN;

    switch (_1st) {
    case 's':
        return SPEC_STR;
    case '%':
        return SPEC_ESCAPE;
    case 'i':
        spec = SPEC_INT;
        break;
    case 'u':
        spec = SPEC_UINT;
        break;
    case 'x':
        spec = SPEC_HEX;
        break;
    default:
        return SPEC_UNKN;
    }

    switch (_2nd) {
    case 'b':
        return spec | SPEC_SZ8;
    case 'w':
        return spec | SPEC_SZ16;
    case 'd':
        return spec | SPEC_SZ32;
    case 'q':
        return spec | SPEC_SZ64;
    }

    return SPEC_UNKN;
}

void kprint(const char* format, ...) {
    va_list args;
    va_start(args, format);

    char* s = NULL;
    char num[MAX_INT64_DIGITS];

    const char* needle = format;
    while (*format) {
        if ('%' != *format) {
            ++format;
            continue;
        }

       if (0 == *(++format)) {
           --format;
           break;
       }
       
       specifier_t spec = specifier_type(format);
       switch (spec) {
       case SPEC_ESCAPE:
           vga_print_range(needle, format - needle);
           break; 
       case SPEC_STR:
           s = va_arg(args, char*);
           PRINT_STR(s)
           break;
       // all casted to int by passing the parameters through an ellipse (...)
       // so just treat them the same way
       case SPEC_INT8:
       case SPEC_INT16:
       case SPEC_INT32:
       case SPEC_UINT8:
       case SPEC_UINT16:
           PRINT_NUM(int64_t, int32_t, itoa);
           break;
       case SPEC_INT64:
           PRINT_NUM(int64_t, int64_t, itoa);
           break;
       case SPEC_UINT32:
           PRINT_NUM(uint64_t, uint32_t, utoa);
           break;
       case SPEC_UINT64:
           PRINT_NUM(uint64_t, uint64_t, utoa);
           break;
       case SPEC_HEX8:
       case SPEC_HEX16:
       case SPEC_HEX32:
           PRINT_NUM(uint64_t, uint32_t, hex);
           break;
       case SPEC_HEX64:
           PRINT_NUM(uint64_t, uint64_t, hex);
           break;
       default:
           vga_print_range(needle, format - needle - 1);
           break;
       }

       ++format;
       needle = format;
    }

    vga_print_range(needle, format - needle);

    va_end(args);
}
