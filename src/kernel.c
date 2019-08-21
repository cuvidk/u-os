#include "string.h"
#include "kprint.h"

void kernel_main() {
    vga_reset_screen();

    const size_t buff_len = VGA_SCREEN_WIDTH * VGA_SCREEN_HEIGHT;
    char test_screen[buff_len + 1];

    test_screen[buff_len] = 0;

    for (size_t i = 0; i < buff_len; ++i) {
        test_screen[i] = '~';
    }

    test_screen[0] = '1';
    test_screen[1] = '2';
    test_screen[2] = '3';
    test_screen[3] = '4';
    test_screen[4] = '5';
    test_screen[buff_len - 5] = '6';
    test_screen[buff_len - 4] = '7';
    test_screen[buff_len - 3] = '8';
    test_screen[buff_len - 2] = '9';
    test_screen[buff_len - 1] = '0';

    vga_print(test_screen);
    //vga_print("\n\n\nc\n");
    //vga_print("stuff");
}
