#include "idt.h"
#include "kprint.h"
#include "trap_frame.h"

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

void resolve_general_exception(trap_frame_t* context, interrupt_t interrupt) {
    kprint("\n--------------- EXCEPTION ENCOUNTERED ---------------\n");

    if (interrupt < 32) {
        kprint("Exception with IDT offset %ud (%s).\n", interrupt, exception_message[interrupt]);
    } else {
        kprint("Unknown exception with IDT offset %ud ?!?!\n", interrupt);
    }

    kprint("\nDumping trap frame:\n");
    dump_trap_frame(context);
    kprint("\n");

    kprint("Halting execution. Bye...\n");
    kprint("-----------------------------------------------------\n");

    asm("cli");
    asm("hlt");
}

extern void _resolve_divide_error();
extern void _resolve_debug_exception();
extern void _resolve_nmi_interrupt();
extern void _resolve_breakpoint();
extern void _resolve_overflow();
extern void _resolve_bound_range_exceeded();
extern void _resolve_invalid_opcode();
extern void _resolve_no_math_coproccessor();
extern void _resolve_double_fault();
extern void _resolve_coprocessor_segment_overrun();
extern void _resolve_invalid_tss();
extern void _resolve_segment_not_present();
extern void _resolve_stack_segment_fault();
extern void _resolve_general_protection();
extern void _resolve_page_fault();
extern void _resolve_math_fault();
extern void _resolve_alignment_check();
extern void _resolve_machine_check();
extern void _resolve_floating_point_exception();
extern void _resolve_virtualization_exception();

void setup_isr() {
    install_interrupt_gate(EXC_DE, _resolve_divide_error);
    install_interrupt_gate(EXC_DB, _resolve_debug_exception);
    install_interrupt_gate(INT_NMI, _resolve_nmi_interrupt);
    install_interrupt_gate(EXC_BP, _resolve_breakpoint);
    install_interrupt_gate(EXC_OF, _resolve_overflow);
    install_interrupt_gate(EXC_BR, _resolve_bound_range_exceeded);
    install_interrupt_gate(EXC_UD, _resolve_invalid_opcode);
    install_interrupt_gate(EXC_NM, _resolve_no_math_coproccessor);
    install_interrupt_gate(EXC_DF, _resolve_double_fault);
    install_interrupt_gate(EXC_CSO, _resolve_coprocessor_segment_overrun);
    install_interrupt_gate(EXC_TS, _resolve_invalid_tss);
    install_interrupt_gate(EXC_NP, _resolve_segment_not_present);
    install_interrupt_gate(EXC_SS, _resolve_stack_segment_fault);
    install_interrupt_gate(EXC_GP, _resolve_general_protection);
    install_interrupt_gate(EXC_PF, _resolve_page_fault);
    install_interrupt_gate(EXC_MF, _resolve_math_fault);
    install_interrupt_gate(EXC_AC, _resolve_alignment_check);
    install_interrupt_gate(EXC_MC, _resolve_machine_check);
    install_interrupt_gate(EXC_XM, _resolve_floating_point_exception);
    install_interrupt_gate(EXC_VE, _resolve_virtualization_exception);
    kprint("    * Added IDT gates for architectural defined exceptions / interrupts\n");
}
