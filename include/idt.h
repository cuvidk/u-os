#ifndef IDT_H_
#define IDT_H_

#include <stdint.h>

void setup_idt();
void install_interrupt_gate(uint8_t offset, void* handler_addr);

#endif /* IDT_H_ */
