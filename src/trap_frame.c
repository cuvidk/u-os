#include "trap_frame.h"
#include "kprint.h"

void dump_trap_frame(trap_frame_t* context) {
    kprint("rax = %xq rbx = %xq rcx = %xq\n", context->rax, context->rbx, context->rcx);
    kprint("rdx = %xq rsp = %xq rbp = %xq\n", context->rdx, context->rsp, context->rbp);
    kprint("rdi = %xq rsi = %xq r8 = %xq\n", context->rdi, context->rsi, context->r8);
    kprint("r9 = %xq r10 = %xq r11 = %xq\n", context->r9, context->r10, context->r11);
    kprint("r12 = %xq r13 = %xq r14 = %xq\n", context->r12, context->r13, context->r14);
    kprint("r15 = %xq\n", context->r15);
    kprint("cr0 = %xq\n", context->cr0);
    kprint("cr2 = %xq\n", context->cr2);
    kprint("cr3 = %xq\n", context->cr3);
    kprint("cr4 = %xq\n", context->cr4);
}
