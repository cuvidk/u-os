#ifndef KPRINT_H_
#define KPRINT_H_

#define VGA_SCREEN_WIDTH 80
#define VGA_SCREEN_HEIGHT 25
#define VGA_BUFFER_ADDR 0xb8000

void vga_reset_screen();
void vga_print(const char* str);

#endif /* KPRINT_H_ */
