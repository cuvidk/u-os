BITS 32

%include "util.h"

%define PAGE_SIZE 0x1000

; ------------------------------------------------ .bss ------------------------------------------------

section .bss
    align PAGE_SIZE
pml4:
    resb PAGE_SIZE
pdpt:
    resb PAGE_SIZE
pd:
    resb PAGE_SIZE
pt:
    ; I'll map only 2MB of memory (identity mapping)
    ; To map 2 MB of memory 512 pages, 4KB in size are needed (2 MB / PAGE_SIZE = 0x200 or 512)
    ; In other words it is enough to have only one PT table since it can contain 512 entries
    resb PAGE_SIZE

; ----------------------------------------------- .text ------------------------------------------------

section .text

global _enable_paging:function (_enable_paging.end - _enable_paging)

; assuming that the processor supports 4 level paging (skipping checks)
_enable_paging:
    ; link the 1st pml4 entry to our only pdpt table
    mov eax, pdpt
    or eax, 0b11 ; present and writeable
    mov [pml4], eax

    ; link the 1st pdpt entry to our only pd table
    mov eax, pd
    or eax, 0b11
    mov [pdpt], eax 

    ; link the 1st pd entry to our only pt table
    mov eax, pt
    or eax, 0b11
    mov [pd], eax

    ; identity map 512 pages of 4KB in size
    xor ecx, ecx
.map_page:
    mov eax, PAGE_SIZE 
    mul ecx                 ; calculate the start addr of the crt page
    or eax, 0b11            ; this page is present and writable
    mov [pt + ecx * 8], eax ; map the page
    inc ecx                 ; inc the number of mapped pages
    cmp ecx, 0x200          ; have we mapped 512 pages ?
    jne .map_page

    ; add reference to pml4 in CR3
    mov eax, pml4
    mov cr3, eax

    ; set PAE bit in CR4
    mov eax, cr4
    or eax, (1 << 5)
    mov cr4, eax

    ; set LME bit in IA32_EFER MSR
    mov ecx, 0xc0000080
    rdmsr
    or eax, (1 << 8)
    wrmsr

    ; set paging bit in cr0
    mov eax, cr0
    or eax, (1 << 31)
    mov cr0, eax

    ret
.end:

