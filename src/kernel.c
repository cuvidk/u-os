#include "vga.h"
#include "kprint.h"
#include "pic.h"
#include "idt.h"
#include "isr.h"
#include "keyboard.h"
#include <limits.h>
#include <stdint.h>

extern void _gen();

void kernel_main() {
    vga_reset_screen();

    kprint("Starting initialization sequence for u-os:\n");

    kprint("\nSetting up PIC\n");
    setup_pic();

    kprint("\nSetting up ISRs\n");
	setup_isr();

    kprint("\nSetting up IDT\n");
    setup_idt();

    // test if IDT interrupt gates are configured
    // correctly triggering a PF by writting to a memory
    // location that is not mapped in the page tables
    //kprint("\nTriggering exception for test reasons\n");
    //*((uint32_t*)0x200000) = 42;

    kprint("\nSetting up keyboard\n");
    setup_keyboard();

    while (1) {}
}
