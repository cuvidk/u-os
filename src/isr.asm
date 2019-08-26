BITS 64

extern _context
extern _save_context
extern resolve_general_exception

%macro DEFINE_INTERRUPT_RESOLVER 2
global %1:function (%1.end - %1)
%1:
    ; we save the context (registers) exactly when the interrupt
    ; hands control to our handler to keep the values as relevant as possible
    call _save_context
    mov rdi, _context
    mov rsi, %2
    call resolve_general_exception
    ; this won't be reach; we halt execution in the above call
    iret
.end:
%endmacro

%define EXC_DE 0
%define EXC_DB 1
%define INT_NMI 2
%define EXC_BP 3
%define EXC_OF 4
%define EXC_BR 5
%define EXC_UD 6
%define EXC_NM 7
%define EXC_DF 8
%define EXC_CSO 9
%define EXC_TS 10
%define EXC_NP 11
%define EXC_SS 12
%define EXC_GP 13
%define EXC_PF 14
%define EXC_MF 16
%define EXC_AC 17
%define EXC_MC 18
%define EXC_XM 19
%define EXC_VE 20

section .text:

DEFINE_INTERRUPT_RESOLVER _resolve_divide_error, EXC_DE
DEFINE_INTERRUPT_RESOLVER _resolve_debug_exception, EXC_DB
DEFINE_INTERRUPT_RESOLVER _resolve_nmi_interrupt, INT_NMI
DEFINE_INTERRUPT_RESOLVER _resolve_breakpoint, EXC_BP
DEFINE_INTERRUPT_RESOLVER _resolve_overflow, EXC_OF
DEFINE_INTERRUPT_RESOLVER _resolve_bound_range_exceeded, EXC_BR
DEFINE_INTERRUPT_RESOLVER _resolve_invalid_opcode, EXC_UD
DEFINE_INTERRUPT_RESOLVER _resolve_no_math_coproccessor, EXC_NM
DEFINE_INTERRUPT_RESOLVER _resolve_double_fault, EXC_DF
DEFINE_INTERRUPT_RESOLVER _resolve_coprocessor_segment_overrun, EXC_CSO
DEFINE_INTERRUPT_RESOLVER _resolve_invalid_tss, EXC_TS
DEFINE_INTERRUPT_RESOLVER _resolve_segment_not_present, EXC_NP
DEFINE_INTERRUPT_RESOLVER _resolve_stack_segment_fault, EXC_SS
DEFINE_INTERRUPT_RESOLVER _resolve_general_protection, EXC_GP
DEFINE_INTERRUPT_RESOLVER _resolve_page_fault, EXC_PF
DEFINE_INTERRUPT_RESOLVER _resolve_math_fault, EXC_MF
DEFINE_INTERRUPT_RESOLVER _resolve_alignment_check, EXC_AC
DEFINE_INTERRUPT_RESOLVER _resolve_machine_check, EXC_MC
DEFINE_INTERRUPT_RESOLVER _resolve_floating_point_exception, EXC_XM
DEFINE_INTERRUPT_RESOLVER _resolve_virtualization_exception, EXC_VE
