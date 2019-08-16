%ifndef VGA_PRINT_H_
%define VGA_PRINT_H_

%define VGA_TEXT_BUFFER 0xB8000

; i'm stupidly assuming this width and height
; however they may be different ???
%define VGA_SCREEN_HEIGHT 25
%define VGA_SCREEN_WIDTH 80

%define VGA_COLOR_BLACK 0
%define VGA_COLOR_BLUE 1
%define VGA_COLOR_GREEN 2
%define VGA_COLOR_WHITE 15

%endif
