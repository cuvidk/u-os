#include <stddef.h>
#include <stdarg.h>
#include "vga.h"

void kprint(const char* format, ...) {
    va_list args;
    va_start(args, format);

    char* s = NULL;

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

       switch (*format) {
       case '%':
           vga_print_range(needle, format - needle);
           ++format;
           break; 
       case 's':
           s = va_arg(args, char*);
           vga_print_range(needle, format - needle - 1);
           vga_print(s);
           ++format;
           break;
       default:
           vga_print_range(needle, format - needle - 1);
           break;
       }

       needle = format;
    }

    vga_print_range(needle, format - needle);

    va_end(args);
}
