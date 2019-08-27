#ifndef PIC_H_
#define PIC_H_

#include <stdint.h>

// set the IDT offsets right after the architectural
// defined exceptions and interrupts
enum {
    IRQ0_TIMER_IDT_OFFSET = 0x20,
    IRQ1_KEYBOARD_IDT_OFFSET,
    IRQ2_CASCADE_IDT_OFFSET,
    IRQ3_SERIAL2_IDT_OFFSET,
    IRQ4_SERIAL1_IDT_OFFSET,
    IRQ5_PARALLEL2_IDT_OFFSET,
    IRQ6_FLOPPY_IDT_OFFSET,
    IRQ7_PARALLEL1_IDT_OFFSET,

    IRQ8_CMOS_RTC_IDT_OFFSET,
    IRQ9_CGA_IDT_OFFSET,
    IRQ10_RESERVED_IDT_OFFSET,
    IRQ11_RESERVED_IDT_OFFSET,
    IRQ12_PS2_AUX_DEVICE_IDT_OFFSET,
    IRQ13_FPU_IDT_OFFSET,
    IRQ14_HDD_IDT_OFFSET,
    IRQ15_RESERVED_IDT_OFFSET
};

enum {
    IRQ0_TIMER,
    IRQ1_KEYBOARD,
    IRQ2_CASCADE,
    IRQ3_SERIAL2,
    IRQ4_SERIAL1,
    IRQ5_PARALLEL2,
    IRQ6_FLOPPY,
    IRQ7_PARALLEL1,
    IRQ8_CMOS_RTC,
    IRQ9_CGA,
    IRQ10_RESERVED,
    IRQ11_RESERVED,
    IRQ12_PS2_AUX_DEVICE,
    IRQ13_FPU,
    IRQ14_HDD,
    IRQ15_RESERVED
};

void setup_pic();
void pic_clear_mask(uint8_t irq);
void pic_set_mask(uint8_t irq);
void pic_send_eoi(uint8_t irq);

#endif /* PIC_H_ */
