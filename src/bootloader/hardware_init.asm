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


; Global symbols
 global check_hardware
 global get_mmap

section .boot

[BITS 16] ; Real mode

 ;Memory descriptor returned by INT 15 
 global mmap_ent
 mmap_ent equ 0x8000             ; the number of entries will be stored at 0x8000


; ====================
; Checking the presence of PCI, CPUID, MSRs
; ====================

check_hardware:
    ; Check CPUID
    call check_cpuid
    ; Check PCI
    call check_pci
    ; Check MSR
    call check_msr

    ret

check_cpuid:
    pushfd ; Saving EFLAGS

    pop eax
    xor eax, 0x200000
    push eax
    popfd
    pushfd
    pop eax

    test eax, 0x200000
    jz .no_cpuid
    ret

.no_cpuid:
    hlt 
    jmp $

check_pci:
    mov eax, 0x80000000
    mov dx, 0xCF8
    out dx, eax
    mov dx, 0xCFC
    in eax, dx

    cmp ax, 0xFFFF
    jz .no_pci
    ret

.no_pci:
    hlt 
    jmp $

check_msr:
    mov eax, 1
    cpuid
    
    test edx, 0x20
    jz .no_msr
    ret

.no_msr:
    hlt 
    jmp $




; ====================
; Getting usable RAM amount
; ====================


get_mmap:
    mov di, 0x8004          ; Set di to 0x8004. Otherwise this code will get stuck in `int 0x15` after some entries are fetched 
	xor ebx, ebx		    ; ebx must be 0 to start
	xor bp, bp		        ; Keep an entry count in bp
	mov edx, 0x534D4150	    ; Place "SMAP" into edx
	mov eax, 0xE820

	mov [es:di + 20], DWORD 1	; Force a valid ACPI 3.X entry
	mov ecx, 24		        ; Ask for 24 bytes
	int 0x15
	jc short .failed	    ; Carry set on first call means "unsupported function"
	mov edx, 0x534D4150	    ; Some BIOSes apparently trash this register?

	cmp eax, edx		    ; On success, eax must have been reset to "SMAP"
	jne short .failed

	test ebx, ebx		    ; ebx = 0 implies list is only 1 entry long (worthless)
	je short .failed

	jmp short .jmpin

.e820lp:
	mov eax, 0xE820		    ; eax, ecx get trashed on every int 0x15 call
	mov [es:di + 20], DWORD 1	; Force a valid ACPI 3.X entry
	mov ecx, 24		        ; Ask for 24 bytes again
	int 0x15
	jc short .e820f		    ; Carry set means "end of list already reached"
	mov edx, 0x534D4150	    ; Repair potentially trashed register

.jmpin:
	jcxz .skipent		    ; Skip any 0 length entries
	cmp cl, 20		        ; Got a 24 byte ACPI 3.X response?
	jbe short .notext
	test byte [es:di + 20], 1	; If so: is the "ignore this data" bit clear?
	je short .skipent
    
.notext:
	mov ecx, [es:di + 8]	; Get lower uint32_t of memory region length
	or ecx, [es:di + 12]	; "or" it with upper uint32_t to test for zero
	jz .skipent		        ; If length uint64_t is 0, skip entry
	inc bp			        ; Got a good entry: ++count, move to next storage spot
	add di, 24

.skipent:
	test ebx, ebx		    ; If ebx resets to 0, list is complete
	jne short .e820lp

.e820f:
	mov [es:mmap_ent], bp	; Store the entry count

	clc			            ; There is "jc" on end of list to this point, so the carry must be cleared
	ret

.failed: ; Infinite loop
	hlt
    jmp $
