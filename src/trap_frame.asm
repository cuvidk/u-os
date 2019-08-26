BITS 64

; ----------------------------------------------- .data ------------------------------------------------ 
section .data:

global _context

_context:
    r_rax dq 0x0
    r_rbx dq 0x0
    r_rcx dq 0x0
    r_rdx dq 0x0
    r_rsp dq 0x0
    r_rbp dq 0x0
    r_rdi dq 0x0
    r_rsi dq 0x0
    r_r8 dq 0x0
    r_r9 dq 0x0
    r_r10 dq 0x0
    r_r11 dq 0x0
    r_r12 dq 0x0
    r_r13 dq 0x0
    r_r14 dq 0x0
    r_r15 dq 0x0
    r_cr0 dq 0x0
    r_cr2 dq 0x0
    r_cr3 dq 0x0
    r_cr4 dq 0x0

; ----------------------------------------------- .text ------------------------------------------------
section .text:

global _save_context:function (_save_context.end - _save_context)

_save_context:
    mov [r_rax], rax
    mov [r_rbx], rbx
    mov [r_rcx], rcx
    mov [r_rdx], rdx
    mov [r_rsp], rsp
    mov [r_rbp], rbp
    mov [r_rdi], rdi
    mov [r_rsi], rsi
    mov [r_r8], r8
    mov [r_r9], r9
    mov [r_r10], r10
    mov [r_r11], r11
    mov [r_r12], r12
    mov [r_r13], r13
    mov [r_r14], r14
    mov [r_r15], r15
    mov rax, cr0
    mov [r_cr0], rax
    mov rax, cr2
    mov [r_cr2], rax
    mov rax, cr3
    mov [r_cr3], rax
    mov rax, cr4
    mov [r_cr4], rax
    ret
.end:
