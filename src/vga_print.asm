[BITS 32]

%include "util.h"
%include "vga_print.h"

global _vga_put_char:function
global _vga_put_str:function
global _vga_clear_screen:function
global _vga_clear_row:function

_vga_put_char:
    xor ebx, ebx
    mov bl, [esp + 0x4] ; character
    mov bh, [esp + 0xc] ; bg color
    shl bh, 0x4
    add bh, [esp + 0x8] ; fg color
    mov eax, [esp + 0x10] ; row
    mov ecx, 2 * VGA_SCREEN_WIDTH ; 2 * WIDTH because 1 char data is stored on 2 bytes
    mul ecx
    mov edx, [esp + 0x14] ; col
    add eax, edx
    add eax, edx ; add 2 * col because 1 char data is store on 2 bytes
    mov edi, VGA_TEXT_BUFFER
    mov [edi + eax], bx
    ret

_vga_clear_row:
    mov edx, [esp + 0x4] ; row
    xor ecx, ecx
.clear_col:
    BITS_32_CALL _vga_put_char, ' ', VGA_COLOR_WHITE, VGA_COLOR_BLACK, edx, ecx
    add ecx, 0x1
    cmp ecx, VGA_SCREEN_WIDTH
    jne .clear_col
    ret
    
_vga_clear_screen:
    mov ecx, 0x0
.clear_row:
    BITS_32_CALL _vga_clear_row, ecx
    add ecx, 0x1
    cmp ecx, VGA_SCREEN_HEIGHT
    jne .clear_row
    ret

_vga_put_str:
    mov esi, [esp + 0x4] ; str addr
    mov ecx, [esp + 0x8] ; length
    mov ebx, [esp + 0xc] ; row
    mov edx, [esp + 0x10] ; col
.should_col_wrap:
    cmp edx, VGA_SCREEN_WIDTH
    jl .should_row_wrap
.col_wrap:
    xor edx, edx
    add ebx, 0x1
.should_row_wrap:
    cmp ebx, VGA_SCREEN_HEIGHT
    jl .put_char
.row_wrap:
    xor ebx, ebx
    xor edx, edx
.put_char:
    cmp edx, 0x0
    jne .skip_clear_row
    BITS_32_CALL _vga_clear_row, ebx
.skip_clear_row:
    BITS_32_CALL _vga_put_char, [esi], VGA_COLOR_WHITE, VGA_COLOR_BLACK, ebx, edx
    add edx, 0x1
    sub ecx, 0x1
    add esi, 0x1
    cmp ecx, 0
    jne .should_col_wrap
    ret
