#include "string.h"
#include "vga.h"
#include "kprint.h"

void kernel_main() {
    vga_reset_screen();

    kprint("715 33z %s bruh (%s) \nescaping %% since 2019, end of the world\nbye, bye", "u-os", "cuv");
    kprint("\nthis % is just ignored");
}
