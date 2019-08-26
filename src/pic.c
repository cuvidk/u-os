#include "pic.h"
#include "io.h"
#include "kprint.h"
#include <stdint.h>

// http://www.brokenthorn.com/Resources/OSDevPic.html

#define PIC_MASTER_COMMAND 0x20
#define PIC_MASTER_DATA (PIC_MASTER_COMMAND + 1)
#define PIC_SLAVE_COMMAND 0xA0
#define PIC_SLAVE_DATA (PIC_SLAVE_COMMAND + 1)

#define PIC_EOI 0x20

#define PIC_ICW1_FLAG_INIT (1 << 4)
#define PIC_ICW1_FLAG_ICW4 (1 << 0)

#define PIC_ICW4_FLAG_8086 (1 << 0)

// set the IDT offsets right after the architectural
// defined exceptions and interrupts
#define PIC_MASTER_IDT_OFFSET 0x20
#define PIC_SLAVE_IDT_OFFSET PIC_MASTER_IDT_OFFSET + 8

static void pic_remap() {
    // save IRQ masks
    uint8_t m = inb(PIC_MASTER_DATA);
    uint8_t s = inb(PIC_SLAVE_DATA);

    // ICW1: enter initialization sequence + specifies that ICW4 will be sent during init
    outb(PIC_MASTER_COMMAND, PIC_ICW1_FLAG_INIT | PIC_ICW1_FLAG_ICW4);
    outb(PIC_SLAVE_COMMAND, PIC_ICW1_FLAG_INIT | PIC_ICW1_FLAG_ICW4);

    // ICW2: remaps IRQs to new IDT offset
    outb(PIC_MASTER_DATA, PIC_MASTER_IDT_OFFSET);
    outb(PIC_SLAVE_DATA, PIC_SLAVE_IDT_OFFSET);

    // ICW3: Specifies what IRQ is connected to slave PIC
    outb(PIC_MASTER_DATA, 0x4); // 0b0000_0100
    // ICW3: IRQ number the master PIC uses to connect to (In binary notation)
    outb(PIC_SLAVE_DATA, 0x2); // 0b0000_0010

    // ICW4: end initialization sequence by setting the PICs in 80x86 mode
    outb(PIC_MASTER_DATA, PIC_ICW4_FLAG_8086);
    outb(PIC_SLAVE_DATA, PIC_ICW4_FLAG_8086);

    // restore masks
    outb(PIC_MASTER_DATA, m);
    outb(PIC_SLAVE_DATA, s);
}

static void pic_clear_mask(uint8_t irq) {
    if (irq > 15) {
        return;
    }

    uint8_t port = PIC_MASTER_DATA;

    if (irq > 7) {
        port = PIC_SLAVE_DATA;
        irq -= 8;
    }

    uint8_t new_mask = inb(port) & ~(1 << irq);
    outb(port, new_mask);
}

void setup_pic() {
    // reprogram the PIC to point to our IVT in the IDT
    pic_remap();
    kprint("    * PIC reprogrammed with IDT offsets between: [%id - %id]\n", 
            PIC_MASTER_IDT_OFFSET,
            PIC_MASTER_IDT_OFFSET + 15);

    // unmask all IRQs so that none is ignored by PIC
    pic_clear_mask(0x0);
    pic_clear_mask(0x1);
    pic_clear_mask(0x2);
    pic_clear_mask(0x3);
    pic_clear_mask(0x4);
    pic_clear_mask(0x5);
    pic_clear_mask(0x6);
    pic_clear_mask(0x7);
    pic_clear_mask(0x8);
    pic_clear_mask(0x9);
    pic_clear_mask(0xa);
    pic_clear_mask(0xb);
    pic_clear_mask(0xc);
    pic_clear_mask(0xd);
    pic_clear_mask(0xf);
    kprint("    * PIC IRQ masks cleared\n");
}

void pic_send_eoi(uint8_t irq) {
    if (irq >= 8)
        outb(PIC_SLAVE_COMMAND, PIC_EOI);
    // the master has to be notified also
    // if the IRQ came from the slave
    outb(PIC_MASTER_COMMAND, PIC_EOI);
}
