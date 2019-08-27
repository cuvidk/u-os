#include "idt.h"
#include "pic.h"
#include "io.h"
#include "kprint.h"

extern void _resolve_keyboard_interrupt();

void setup_keyboard() {
    install_interrupt_gate(IRQ1_KEYBOARD_IDT_OFFSET, _resolve_keyboard_interrupt);
    kprint("    * Installed keyboard IDT gate\n");

    pic_clear_mask(IRQ1_KEYBOARD);
    kprint("    * Clearing IRQ1 mask\n");
}

void resolve_keyboard_interrupt() {
    pic_send_eoi(IRQ1_KEYBOARD);
    
    uint8_t code = inb(0x64);
    uint8_t key = inb(0x60);

    kprint("keyboard interrupt received\n");
}
