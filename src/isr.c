#include "idt.h"
#include "kprint.h"

typedef enum {
    EXC_DE = 0,
    EXC_DB,
    INT_NMI,
    EXC_BP,
    EXC_OF,
    EXC_BR,
    EXC_UD,
    EXC_NM,
    EXC_DF,
    EXC_CSO,
    EXC_TS,
    EXC_NP,
    EXC_SS,
    EXC_GP,
    EXC_PF,
    EXC_MF = 16,
    EXC_AC,
    EXC_MC,
    EXC_XM,
    EXC_VE
} interrupt_t;

static const char *exception_message[] = {
    "Divide Error",
    "Debug Exception",
    "NMI Interrupt",
    "Breakpoint",
    "Overflow",
    "Bound Range Exceeded",
    "Invalid Opcode",
    "No Math Coprocessor",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack-Segment Fault",
    "General Protection",
    "Page Fault",
    "Reserved",
    "x87 FPU Floating-Point Error (Math Fault)",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating-Point Exception",
    "Virtualization Exception",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

static void resolve_general_exception(interrupt_t interrupt) {
    kprint("\n--------------- EXCEPTION ENCOUNTERED ---------------\n");

    if (interrupt < 32) {
        kprint("Exception with IDT offset %ud (%s).\n", interrupt, exception_message[interrupt]);
    } else {
        kprint("Unknown exception with IDT offset %ud ?!?!\n", interrupt);
    }

    //dump_frame

    kprint("Halting execution. Bye...\n");
    kprint("-----------------------------------------------------\n");

    asm("cli");
    asm("hlt");
}

static void resolve_divide_error()                { resolve_general_exception(EXC_DE); }
static void resolve_debug_exception()             { resolve_general_exception(EXC_DB); }
static void resolve_nmi_interrupt()               { resolve_general_exception(INT_NMI); }
static void resolve_breakpoint()                  { resolve_general_exception(EXC_BP); }
static void resolve_overflow()                    { resolve_general_exception(EXC_OF); }
static void resolve_bound_range_exceeded()        { resolve_general_exception(EXC_BR); }
static void resolve_invalid_opcode()              { resolve_general_exception(EXC_UD); }
static void resolve_no_math_coproccessor()        { resolve_general_exception(EXC_NM); }
static void resolve_double_fault()                { resolve_general_exception(EXC_DF); }
static void resolve_coprocessor_segment_overrun() { resolve_general_exception(EXC_CSO); }
static void resolve_invalid_tss()                 { resolve_general_exception(EXC_TS); }
static void resolve_segment_not_present()         { resolve_general_exception(EXC_NP); }
static void resolve_stack_segment_fault()         { resolve_general_exception(EXC_SS); }
static void resolve_general_protection()          { resolve_general_exception(EXC_GP); }
static void resolve_page_fault()                  { resolve_general_exception(EXC_PF); }
static void resolve_math_fault()                  { resolve_general_exception(EXC_MF); }
static void resolve_alignment_check()             { resolve_general_exception(EXC_AC); }
static void resolve_machine_check()               { resolve_general_exception(EXC_MC); }
static void resolve_floating_point_exception()    { resolve_general_exception(EXC_XM); }
static void resolve_virtualization_exception()    { resolve_general_exception(EXC_VE); }

void setup_isr() {
    install_interrupt_gate(EXC_DE, resolve_divide_error);
    install_interrupt_gate(EXC_DB, resolve_debug_exception);
    install_interrupt_gate(INT_NMI, resolve_nmi_interrupt);
    install_interrupt_gate(EXC_BP, resolve_breakpoint);
    install_interrupt_gate(EXC_OF, resolve_overflow);
    install_interrupt_gate(EXC_BR, resolve_bound_range_exceeded);
    install_interrupt_gate(EXC_UD, resolve_invalid_opcode);
    install_interrupt_gate(EXC_NM, resolve_no_math_coproccessor);
    install_interrupt_gate(EXC_DF, resolve_double_fault);
    install_interrupt_gate(EXC_CSO, resolve_coprocessor_segment_overrun);
    install_interrupt_gate(EXC_TS, resolve_invalid_tss);
    install_interrupt_gate(EXC_NP, resolve_segment_not_present);
    install_interrupt_gate(EXC_SS, resolve_stack_segment_fault);
    install_interrupt_gate(EXC_GP, resolve_general_protection);
    install_interrupt_gate(EXC_PF, resolve_page_fault);
    install_interrupt_gate(EXC_MF, resolve_math_fault);
    install_interrupt_gate(EXC_AC, resolve_alignment_check);
    install_interrupt_gate(EXC_MC, resolve_machine_check);
    install_interrupt_gate(EXC_XM, resolve_floating_point_exception);
    install_interrupt_gate(EXC_VE, resolve_virtualization_exception);
    kprint("    * Added IDT gates for architectural defined exceptions / interrupts\n");
}
