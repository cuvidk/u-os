#include "string.h"
#include "vga.h"
#include "kprint.h"
#include "pic.h"

#include <limits.h>
#include <stdint.h>

void kernel_main() {
    vga_reset_screen();

    pic_init();

    kprint("715 33z %s bruh (%s) \nescaping %% since 2019, end of the world\nbye, bye", "u-os", "cuv");
    kprint("\nthis % is just ignored\n");
    kprint("n'this 2 %w");
    kprint("");
    kprint("int8_t: %ib and heres %iw some huge int: %iq\n", -22, -2322, LONG_MIN);

    uint64_t max = ULONG_MAX;
    kprint("oh heres a huge uint: %uq\n", max);
    kprint("%uq\n", 0);

    kprint("hex test: %xq\n", ULONG_MAX);
    kprint("hex test: %xb\n", 0);
    kprint("hex test: %xd\n", INT_MIN);
    kprint("hex test: %xd\n", INT_MAX);
    kprint("hex test: %xd\n", 5412449);
    kprint("simple string, no other shit\n");
}
