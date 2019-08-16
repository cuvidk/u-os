BITS 32 ; i'm forcing the assembler to gen 32bit code even though i'm compiling with -felf64
        ; that's because multiboot compliant bootloaders will start executing the kernel code
        ; in 32 bit mode

%include "vga_print.h"
%include "util.h"

extern _vga_put_str
extern _vga_clear_screen

%define MBOOT_HEADER_ALIGN (1 << 0) ; tells bootloader that all modules loaded should be 4KB aligned
%define MBOOT_HEADER_MMAP (1 << 1)  ; tells bootloader that a memory map should be provided
%define MBOOT_HEADER_FLAGS (MBOOT_HEADER_ALIGN | MBOOT_HEADER_MMAP)
%define MBOOT_HEADER_MAGIC 0x1badb002
%define MBOOT_HEADER_CHECKSUM (0 - MBOOT_HEADER_FLAGS - MBOOT_HEADER_MAGIC)

section .multiboot
    align 4
    dd MBOOT_HEADER_MAGIC
    dd MBOOT_HEADER_FLAGS
    dd MBOOT_HEADER_CHECKSUM

section .data
    kern_msg db 'hello world'
    kern_msg_len equ $ - kern_msg

section .bss
    align 16
stack_limit:
    resb 16384
stack_top:

section .text
global _start:function (_start.end - _start)
; bootloader will hand control to this
; function. it also handles transfer to
; 32 bit mode (if the bootloader is multiboot compliant)
_start:
    mov esp, stack_top
    mov ebp, esp

    BITS_32_CALL _vga_put_str, kern_msg, kern_msg_len, 0, 0

;    extern kernel_main
;    call kernel_main
    
    cli
.hang:
    hlt
    jmp .hang
.end:
