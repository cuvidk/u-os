BITS 32 ; i'm forcing the assembler to gen 32bit code even though i'm compiling with -felf64
        ; that's because multiboot compliant bootloaders will start executing the kernel code
        ; in 32 bit mode

%include "vga_print.h"
%include "util.h"

extern _vga_clear_screen
extern _vga_print_cstr
extern _setup_gdt32
extern _setup_gdt64
extern _enable_paging

extern kernel_main

%define MBOOT_HEADER_ALIGN (1 << 0) ; tells bootloader that all modules loaded should be 4KB aligned
%define MBOOT_HEADER_MMAP (1 << 1)  ; tells bootloader that a memory map should be provided
%define MBOOT_HEADER_FLAGS (MBOOT_HEADER_ALIGN | MBOOT_HEADER_MMAP)
%define MBOOT_HEADER_MAGIC 0x1badb002
%define MBOOT_HEADER_CHECKSUM (0 - MBOOT_HEADER_FLAGS - MBOOT_HEADER_MAGIC)

%define LOG(m_addr) BITS_32_CALL _vga_print_cstr, m_addr

; --------------------------------------------- .multiboot ---------------------------------------------

section .multiboot
    align 4
    dd MBOOT_HEADER_MAGIC
    dd MBOOT_HEADER_FLAGS
    dd MBOOT_HEADER_CHECKSUM

; ----------------------------------------------- .data ------------------------------------------------ 

section .data
    hello_boot     db 'Starting system initialization:', 0
    info_gdt32     db '   * Setting up 32 bit GDT', 0
    info_paging    db '   * Enabling 4 level paging', 0
    info_long_mode db '   * Setting up 64 bit GDT and transitioning to long mode', 0

; ------------------------------------------------ .bss ------------------------------------------------

section .bss
    align 16
stack_limit:
    resb 16384
stack_top:

; ----------------------------------------------- .text ------------------------------------------------

section .text

global _start:function (_start.end - _start)

_start:
    LOG(hello_boot)

    ; setup a stack
    mov esp, stack_top
    mov ebp, esp

    LOG(info_gdt32)
    call _setup_gdt32

    LOG(info_paging)
    call _enable_paging

    LOG(info_long_mode)
    call _setup_gdt64

BITS 64
    sti

    call kernel_main

    cli
.hang:
    hlt
    jmp .hang
.end:

; ------------------------------------------------------------------------------------------------------
