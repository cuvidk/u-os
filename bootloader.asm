[BITS 16]
[ORG 0x7c00]
boot_loader_start:
    mov ah, 0x00 ; set the video mode
    mov al, 0x03 ; set desired video mode
    int 0x10 ; video software interrupt

    mov sp, boot_loader_start ; sets up stack at addr(boot_loader_start) and growing towards 0
    mov bp, sp ; stack frame == stack pointer for the moment, nothing to deallocate

    push 0x19 ; collumn
    push 0x0b ; row
    push hello_str ; message we want to display
    push HELLO_STR_LEN

dump_string:

    mov dh, [bp - 0x4] ; set initial position (row)
    mov dl, [bp - 0x2] ; +8 because pushing a value on stack pushes 16 bits

    mov si, 0
update_screen_position:
    mov ah, 0x02 ; set the cursor position
    mov bh, 0x00 ; set page to display text on
    int 0x10 ; video software interrupt

dump_letter:
    mov bx, [bp - 0x6]

    mov ah, 0x0a
    mov al, [bx + si]
    mov bh, 0x00
    mov bl, 0x00
    mov cx, 0x01
    int 0x10

    add dl, 1

    add si, 1
    cmp si, [bp - 0x8]
    jne update_screen_position


hello_str db 'Hello from bootloader !'
HELLO_STR_LEN equ $-hello_str

times 510 - ($-$$) db 0
db 0x55,0xaa
times 1474560 - ($-$$) db 0
