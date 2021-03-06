#ifndef TRAP_FRAME_H_
#define TRAP_FRAME_H_

#include <stdint.h>

typedef struct {
    uint64_t rax;
    uint64_t rbx;
    uint64_t rcx;
    uint64_t rdx;
    uint64_t rsp;
    uint64_t rbp;
    uint64_t rdi;
    uint64_t rsi;
    uint64_t r8;
    uint64_t r9;
    uint64_t r10;
    uint64_t r11;
    uint64_t r12;
    uint64_t r13;
    uint64_t r14;
    uint64_t r15;
    uint64_t cr0;
    uint64_t cr2;
    uint64_t cr3;
    uint64_t cr4;
} __attribute__((packed)) trap_frame_t;


void dump_trap_frame(trap_frame_t* context);

#endif /* TRAP_FRAME_H_ */
