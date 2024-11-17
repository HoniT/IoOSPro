; =======================================================================
; Copyright Ioane Baidoshvili 2024.
; Distributed under the terms of the MIT License.
; (See accompanying file LICENSE or copy at
; http://www.opensource.org/licenses/MIT_1_0.txt)
; 
; hardware_init.asm manages information about hardware
; This file contains: 
; Checking the presence of PCI, CPUID, MSRs and gets hardware information
; =======================================================================

; ====================
; Checking the presence of PCI, CPUID, MSRs
; ====================

[BITS 16]

section .data
    memory_buffer resb 32 * 24 ; Reserving space for 32 entries

    ; Error messages
    cpuid_not_supported db 'CPUID not supported!', 0
    pci_not_supported db 'PCI not supported!', 0
    msr_not_supported db 'MSR not supported!', 0


section .boot
    cpuid_supported db 0   ; Flag for CPUID support (0: not supported, 1: supported)
    pci_supported db 0     ; Flag for PCI support (0: not supported, 1: supported)
    msr_supported db 0     ; Flag for MSR support (0: not supported, 1: supported)



    global check_hardware
    global get_ram_amount
    extern print_str_realmode
    extern print_string_pm

check_hardware:

    mov esi, cpuid_not_supported
    call print_str_realmode

    ; Checking CPUID
    call check_cpuid
    ; Checking PCI
    call check_pci
    ; Checking MSRs
    call check_msr


    ; Checking if anything is not supported
    cmp byte [cpuid_supported], 0 ; Check if cpuid_supported is 1
        je cpuid_n_supported            ; Jump if CPUID is not supported

    cmp byte [pci_supported], 0 ; Check if pci_supported is 1
        je pci_n_supported            ; Jump if PCI is not supported

    cmp byte [msr_supported], 0 ; Check if msr_supported is 1
        je msr_n_supported            ; Jump if MSR is not supported

    ret


; Prints error messages

cpuid_n_supported:
    mov esi, cpuid_not_supported
    call print_str_realmode

    hlt
    jmp $

pci_n_supported:
    mov esi, pci_not_supported
    call print_str_realmode

    hlt
    jmp $

msr_n_supported:
    mov esi, msr_not_supported
    call print_str_realmode

    hlt 
    jmp $


check_cpuid:
    pushfd                    ; Save EFLAGS to stack
    pop eax                   ; Move EFLAGS to EAX

    mov ecx, eax              ; Copy EFLAGS
    xor eax, 0x200000         ; Toggle ID bit (21st bit)

    push eax                  ; Set the new EFLAGS
    popfd                     ; Load back to EFLAGS
    pushfd                    ; Save EFLAGS again
    pop eax                   ; Move it to EAX
    xor eax, ecx              ; Compare with original EFLAGS
    jz .no_cpuid              ; If ID bit didn’t toggle, CPUID is not supported

    mov byte [cpuid_supported], 1 ; CPUID is supported
    ret

.no_cpuid:
    ret

check_pci:
    mov eax, 0x80000000       ; Bus 0, device 0, function 0
    mov dx, 0xCF8             ; PCI configuration address port
    out dx, eax               ; Write to the PCI configuration space
    mov dx, 0xCFC             ; PCI configuration data port
    in eax, dx                ; Read the vendor ID from PCI config data port
    cmp ax, 0xFFFF            ; Check if vendor ID is 0xFFFF
    je .no_pci                ; If vendor ID is 0xFFFF, no PCI support

    mov byte [pci_supported], 1 ; PCI is supported
    ret

.no_pci:
    ret

check_msr:
    ; Ensure CPUID is supported before checking MSR
    cmp byte [cpuid_supported], 0
    je .no_msr                ; Skip if CPUID is not supported

    mov eax, 1                ; CPUID function 1
    cpuid
    test edx, (1 << 5)        ; Check if the 5th bit of EDX is set
    jz .no_msr                ; If not set, MSR is not supported

    mov byte [msr_supported], 1 ; MSR is supported
    ret

.no_msr:
    ret


get_ram_amount:

    xor ebx, ebx ; Zeroing ebx

memory_loop:
    mov eax, 0xE820        ; INT 0x15 E820 function
    mov ecx, 24            ; Size of the structure (in bytes)
    mov edx, 0x534D4150    ; 'SMAP' identifier, thi is needed to recognize it as a memory map request 

    ; Calculate the address of buffer + ebx * 24 in esi
    mov esi, memory_buffer      ; Load base address of buffer into ESI
    mov edi, esi           ; Copy buffer's address into EDI
    add edi, ebx           ; Add ebx (index) to edi
    shl edi, 3             ; Multiply index by 8 (to achieve ebx * 24 addressing)

    int 0x15               ; BIOS call for memory map
    jc end_loop            ; If carry flag is set, no more entries

    ; Check if this entry is usable memory

    
    cmp dword [edi], 1  ; Check if "type" is 1
    jne skip_entry
    ; If type is 1, add the memory size to a total (in another register)
    ; (example code for summing size omitted here)

skip_entry:
    inc ebx                ; Move to the next entry
    jmp memory_loop

end_loop:
    ; End of memory map retrieval
    mov esi, memory_buffer     ; Address of memory buffer to print
    call print_str_realmode   ; Print the memory buffer (if relevant)