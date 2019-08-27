%include "util.h"
BITS 64

extern resolve_keyboard_interrupt

section .text:

global _resolve_keyboard_interrupt:function (_resolve_keyboard_interrupt.end - _resolve_keyboard_interrupt)
_resolve_keyboard_interrupt:
    cli

    push        rax
    push        rbx
    push        rcx
    push        rdx
    push        r8
    push        r9
    push        r10
    push        r11
    push        r12
    push        r13
    push        r14
    push        r15

    call resolve_keyboard_interrupt

    pop         r15
    pop         r14
    pop         r13
    pop         r12
    pop         r11
    pop         r10
    pop         r9
    pop         r8
    pop         rdx
    pop         rcx
    pop         rbx
    pop         rax

    sti
    iretq
.end
