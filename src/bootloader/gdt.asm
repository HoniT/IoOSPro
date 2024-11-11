; =======================================================================
; Copyright Ioane Baidoshvili 2024.
; Distributed under the terms of the MIT License.
; (See accompanying file LICENSE or copy at
; http://www.opensource.org/licenses/MIT_1_0.txt)
; 
; gdt.asm sets up the Global Descriptor Table
; This file contains: 
; Setting up the GDT
; =======================================================================

[BITS 16]

section .boot 
    global gdt_descriptor
    global CODE_SEG_OFFSET
    global DATA_SEG_OFFSET

    ; GDT segments
    CODE_SEG_OFFSET equ gdt_code_seg - gdt_start
    DATA_SEG_OFFSET equ gdt_data_seg - gdt_start


gdt_start:
    ; Null descriptor
    dd 0x0
    dd 0x0

; Code segment descriptor
gdt_code_seg:
    dw 0xFFFF      ; Limit
    dw 0x0000      ; Base address low
    db 0x00        ; Base address middle
    db 10011010b   ; Access rights: executable, readable, non-conforming, etc.
    db 11001111b   ; Flags: 32-bit, 4K granularity
    db 0x00        ; Base high

; Data segment descriptor
gdt_data_seg:
    dw 0xFFFF      ; Limit
    dw 0x0000      ; Base address low
    db 0x00        ; Base address middle
    db 10010010b   ; Access rights: writable, non-executable
    db 11001111b   ; Flags: 32-bit, 4K granularity
    db 0x00        ; Base high

gdt_end:
    ; For calculating gdt size

gdt_descriptor:
    dw gdt_end - gdt_start - 1 ; GDT size
    dd gdt_start               ; GDT base address