; =======================================================================
; Copyright Ioane Baidoshvili 2024.
; Distributed under the terms of the MIT License.
; (See accompanying file LICENSE or copy at
; http://www.opensource.org/licenses/MIT_1_0.txt)
; 
; util.asm contains utility functions
; This file contains: 
; Real mode print functions
; =======================================================================

section .boot
    global print_str_realmode
    global print_string_pm

print_str_realmode:
    .next_char:
        lodsb           ; Load the next byte from DS:SI into AL and increment SI
        cmp al, 0       ; Check if the character is null (end of string)
        je .done        ; If it is null, jump to done

        ; Call BIOS interrupt to print the character
        mov ah, 0x0E    ; Function to write a character in teletype mode
        int 0x10        ; Call the BIOS interrupt
        jmp .next_char  ; Loop to the next character

    .done:
        ret             ; Return from the function
