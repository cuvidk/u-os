%ifndef UTIL_ASM_
%define UTIL_ASM_

%macro PROLOGUE 0
    push ebp
    mov ebp, esp
%endmacro

%macro EPILOGUE 0
    mov esp, ebp
    pop ebp
    ret
%endmacro EPILOGUE

; convenience macro which prepares the stack
; for a function call. arguments for the function
; to call ar pushed right to left. it also handles
; stack clean-up on return
%macro BITS_32_CALL 1-*
    pushad
    %rep %0-1
    %rotate -1
        push DWORD %1
    %endrep
    %rotate -1
    call %1
    add esp, (%0 - 1) * 4
    popad
%endmacro BITS_32_CALL

%macro BREAKPOINT 0
    cli
.breakpoint:
    hlt
    jmp .breakpoint
%endmacro BREAKPOINT

%endif ;UTIL_ASM_
