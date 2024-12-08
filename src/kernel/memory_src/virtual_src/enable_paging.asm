; =======================================================================
; Copyright Ioane Baidoshvili 2024.
; Distributed under the terms of the MIT License.
; (See accompanying file LICENSE or copy at
; http://www.opensource.org/licenses/MIT_1_0.txt)
; 
; enable_paging.asm enables paging
; =======================================================================

section .text
    global enable_paging

enable_paging:
    ; Enabling PSE
    mov eax, cr4
    or eax, 0x10
    mov cr4, eax

    mov eax, [esp + 4]
    mov cr3, eax

    mov eax, cr0
    or eax, 0x80000001
    mov cr0, eax

    ret
