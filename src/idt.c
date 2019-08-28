#include "idt.h"
#include "kprint.h"

#define RING_0 0x0
#define RING_1 0x1
#define RING_2 0x2
#define RING_3 0x3

#define TYPE_INTERRUPT 0xe 
// we've set this to offset 0x8 when we configured gdt64
#define KERNEL_CODE_SEGMENT_SELECTOR 0x8

typedef struct {
    uint16_t offset_15_0;
    uint16_t segment_selector;
    uint8_t interrupt_stack_table : 3;
    uint8_t zero_1 : 5;
    uint8_t type : 4;
    uint8_t zero_2 : 1;
    uint8_t dpl : 2;
    uint8_t present : 1;
    uint16_t offset_31_16;
    uint32_t offset_63_32;
    uint32_t zero_3;
} __attribute__((packed)) idt_entry_t;

typedef struct {
    uint16_t size;
    uint64_t base : 48;
} __attribute__((packed)) idt_desc_t;


idt_entry_t idt[256];
idt_desc_t idt_desc;

void install_interrupt_gate(uint8_t offset, void* handler_addr) {
    idt[offset].offset_15_0 = (uint64_t)handler_addr & 0xffff;
    idt[offset].offset_31_16 = ((uint64_t)handler_addr >> 16) & 0xffff;
    idt[offset].offset_63_32 = ((uint64_t)handler_addr >> 32) & 0xffffffff;
    idt[offset].segment_selector = KERNEL_CODE_SEGMENT_SELECTOR;
    idt[offset].zero_1 = 0x0;
    idt[offset].zero_2 = 0x0;
    idt[offset].zero_3 = 0x0;
    idt[offset].interrupt_stack_table = 0x0;
    idt[offset].type = TYPE_INTERRUPT;
    idt[offset].dpl = RING_0;
    idt[offset].present = 0x1;
}

void setup_idt() {
    idt_desc.size = sizeof(idt) - 1;
    idt_desc.base = (uint64_t)idt;

    kprint("\t* Setting IDT table at addr %xq\n", &idt);
    kprint("\t* Setting IDT size to %uq\n", idt_desc.size);

    asm ("lidt %0" : : "m"(idt_desc));
    kprint("\t* Loaded new IDT table pointer in IDTR register\n");
}
