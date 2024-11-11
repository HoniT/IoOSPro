; =======================================================================
; Copyright Ioane Baidoshvili 2024.
; Distributed under the terms of the MIT License.
; (See accompanying file LICENSE or copy at
; http://www.opensource.org/licenses/MIT_1_0.txt)
; 
; gdt_def.asm sets up the Global Descriptor Table in the kernel
; This file contains: 
; Loading the GDT and TSS
; =======================================================================

[BITS 32]

section .text
    global gdt_flush
    global tss_flush

gdt_flush:
    ; Loading GDT
    mov eax, [esp + 4]
    lgdt [eax]

    ; Setting segments
    mov ax, 0x10 ; Data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    jmp 0x08:.flush ; Far jump using Code Segment

    .flush:
        ret

tss_flush:
    mov ax, 0x2B ; TSS segment in GDT (6th segment)
    ltr ax

    ret
    