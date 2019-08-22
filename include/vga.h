#ifndef VGA_H_
#define VGA_H_

#include <stddef.h>

#define VGA_SCREEN_WIDTH 80
#define VGA_SCREEN_HEIGHT 25
#define VGA_BUFFER_ADDR 0xb8000

void vga_reset_screen();
void vga_print(const char* str);
void vga_print_range(const char* str, size_t len);

#endif /* VGA_H_ */
