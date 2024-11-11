; =======================================================================
; Copyright Ioane Baidoshvili 2024.
; Distributed under the terms of the MIT License.
; (See accompanying file LICENSE or copy at
; http://www.opensource.org/licenses/MIT_1_0.txt)
; 
; kernel_entry.asm gives control to the kernel
; This file contains: 
; Jumping to kernel_main.cpp
; =======================================================================


[BITS 32] ; We are in protected mode

    global _start 
    extern kernel_main ; External symbol of kernel_main.cpp

_start:

    call kernel_main ; Connecting to kernel_main.cpp (void kernel_main())
    
hltloop:
    hlt 
    jmp hltloop