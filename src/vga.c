#include <stdint.h>
#include "vga.h"
#include "string.h"

static uint16_t* vga_buffer = (uint16_t*)VGA_BUFFER_ADDR;
static short crt_row;
static short crt_col;

typedef enum {
    VGA_COLOR_BLACK,
    VGA_COLOR_BLUE,
    VGA_COLOR_GREEN,
    VGA_COLOR_WHITE = 15
} vga_color_t;

static uint8_t vga_encode_color(vga_color_t fg, vga_color_t bg) {
    return (uint8_t)(bg << 4 | fg);
}

static uint8_t vga_default_color() {
    return vga_encode_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
}

static uint16_t vga_encode_char(char c, uint8_t color) {
    return (uint16_t) (color << 8 | c);
}

static void vga_clear_screen() {
    size_t buffer_size = VGA_SCREEN_WIDTH * VGA_SCREEN_HEIGHT;

    for (size_t i = 0; i < buffer_size; ++i) {
        vga_buffer[i] = vga_encode_char(' ', vga_default_color());
    }
}

static void vga_scroll_line() {
    uint16_t* src = vga_buffer + VGA_SCREEN_WIDTH;
    size_t buffer_size = VGA_SCREEN_WIDTH * VGA_SCREEN_HEIGHT;
    size_t scroll_size = buffer_size - VGA_SCREEN_WIDTH;

    size_t i = 0;
    for (; i < scroll_size; ++i) {
        vga_buffer[i] = *src;
        ++src;
    }

    for (; i < buffer_size; ++i) {
        vga_buffer[i] =  vga_encode_char(' ', vga_default_color());
    }
}

void vga_reset_screen() {
    vga_clear_screen();
    crt_row = 0;
    crt_col = 0;
}

void vga_print_range(const char* str, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        if ('\n' == str[i]) {
            ++crt_row;
            crt_col = 0;
            continue;
        }

        if (VGA_SCREEN_WIDTH <= crt_col) {
            ++crt_row;
            crt_col = 0;
        }

        if (VGA_SCREEN_HEIGHT <= crt_row) {
            size_t num_scrolls = crt_row - VGA_SCREEN_HEIGHT + 1;
            while (num_scrolls) {
                vga_scroll_line();
                --num_scrolls;
                --crt_row;
            }
            crt_col = 0;
        }

        uint16_t c = vga_encode_char(str[i], vga_default_color());
        vga_buffer[crt_row * VGA_SCREEN_WIDTH + crt_col] =  c;

        ++crt_col;
    }
}

void vga_print(const char* str) {
    size_t len = strlen(str);
    vga_print_range(str, len);
}
