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

; --------------------------------------------- .multiboot ---------------------------------------------

section .multiboot
    align 4
    dd MBOOT_HEADER_MAGIC
    dd MBOOT_HEADER_FLAGS
    dd MBOOT_HEADER_CHECKSUM

; ----------------------------------------------- .data ------------------------------------------------ 

section .data
; GDT is already 8 byte aligned (actually 4KB aligned) due to the
; linker that specifies that the data segment should be 4KB alligned
gdt: 
.null_seg_desc:
    dq 0x0
.code_seg_desc:
    dw 0xffff       ; segment limit (b0 - b15)
    dw 0x0000       ; base address (b0 - b15)
    db 0x00         ; base address (b16 - b23)
    db 0b1001_1010  ; 0: present byte, 1-2: DPL, 3: desc type (sys / code or data), 4-7: type
    db 0b1100_1111  ; 0: granularity, 1: D/B, 2: L, 3: AVL, 4-7: segment limit (b16 - b19)
    db 0x00         ; base address (b24 - b31)
                    ; see 3.4.5 Intel_V3 for more info
.data_seg_desc:
    dw 0xffff
    dw 0x0000
    db 0x00
    db 0b1001_0010
    db 0b1100_1111
    db 0x00
.end

gdt_desc:
    dw gdt.end - gdt ; - 1 byte ????
    dd gdt

str_data:
    kern_msg db 'hello world'
    kern_msg_len equ $ - kern_msg


section .bss
    ; system v abi requires the stack to be 16 byte aligned
    align 16
stack_limit:
    resb 16384
stack_top:

; ----------------------------------------------- .text ------------------------------------------------

section .text
global _start:function (_start.end - _start)
_start:
    ; The multiboot compliant bootloader already drops us in 32 bit protected mode.
    ; The bootloader is required to handle only the initialization of the segment
    ; registers, however it is stated that GDTR may be invalid (it seems GRUB actually
    ; provides a valid GDTR). Taking into account all that, I'm creating my own GDT
    ; just to be sure all's good and why not for practice reasons.
    lgdt [gdt_desc]
    jmp (gdt.code_seg_desc - gdt):.reload_segments
.reload_segments:
    mov ax, (gdt.data_seg_desc - gdt)
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov esp, stack_top
    mov ebp, esp

    BITS_32_CALL _vga_put_str, kern_msg, kern_msg_len, 0, 0

    cli
.hang:
    hlt
    jmp .hang
.end:

; ------------------------------------------------------------------------------------------------------
