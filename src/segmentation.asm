BITS 32

; ----------------------------------------------- .data ------------------------------------------------ 

section .data

    align 8
gdt32: 
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
.end:

gdt32_desc:
    dw gdt32.end - gdt32 - 1
    dd gdt32

    align 8
gdt64:
.null_seg_desc:
    dq 0x0
.code_seg_desc:
    dw 0xffff       
    dw 0x0000       
    db 0x00         
    db 0b1001_1010
    db 0b1010_1111 ; the only difference from gdt32 code descriptor is that D/B flag is cleared and L flag is set
    db 0x00         
; not definining a data segment descriptor
; since it would be useless in long mode
; will point all data segments to the null seg desc
.end:

gdt64_desc:
    dw gdt64.end - gdt64 - 1
    dq gdt64
                    

; ----------------------------------------------- .text ------------------------------------------------

section .text

global _setup_gdt32:function (_setup_gdt32.end - _setup_gdt32)
global _setup_gdt64:function (_setup_gdt64.end - _setup_gdt64)

_setup_gdt32:
    ; The multiboot compliant bootloader already drops us in 32 bit protected mode.
    ; The bootloader is required to handle only the initialization of the segment
    ; registers, however it is stated that GDTR may be invalid (it seems GRUB actually
    ; provides a valid GDTR). Taking into account all that, I'm creating my own GDT
    ; just to be sure all's good and why not for practice reasons.
    lgdt [gdt32_desc]
    jmp (gdt32.code_seg_desc - gdt32):.reload_segments
.reload_segments:
    mov ax, (gdt32.data_seg_desc - gdt32)
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    ; No need to transition in protected mode since the multiboot
    ; compliant bootloader already handles that
    ret
.end:

_setup_gdt64:
    lgdt [gdt64_desc]
    ; this reloads cs with the new code segment descriptor
    ; completing the transition into long mode
    jmp (gdt64.code_seg_desc - gdt64):.reload_segments
BITS 64
.reload_segments:
    ; we should be in long mode here
    ; reload the rest of the segment registers with null descriptor offset
    mov ax, gdt64.null_seg_desc - gdt64
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; unfortunatelly we cannot return gracefully from this.
    ; we are in long mode now, meaning that the ret instruction will
    ; actually pop 64 bits from the stack instead of 32, resulting
    ; in an invalid address.
    ;
    ; Temporary solution (hack): Set rcx to 0 and mov 32 bits from the 
    ; stack (32 bit ret address) in register ecx. That should resolve to
    ; valid 64 bit address. Just perform a jump to that address after
    ; restoring the memory allocated on the stack
    xor rcx, rcx
    mov ecx, [esp]
    add esp, 0x4
    jmp rcx
.end:
