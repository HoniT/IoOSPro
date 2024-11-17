; =======================================================================
; Copyright Ioane Baidoshvili 2024.
; Distributed under the terms of the MIT License.
; (See accompanying file LICENSE or copy at
; http://www.opensource.org/licenses/MIT_1_0.txt)
; 
; boot.asm is the main file of the IoOS bootloader
; This file contains: 
; Loading GDT, getting hardware information, enabling A20 line, loading kernel to memory,
; switching to protected mode, giving kernel control
; =======================================================================

; External symbols, variables, addresses, offsets
section .boot

    extern check_hardware
    extern get_ram_amount
    extern gdt_descriptor
    extern _start
    extern print_str_realmode

    extern CODE_SEG_OFFSET
    extern DATA_SEG_OFFSET

    extern __kernel_sectors

    ; Kernel locations and offset
    KERNEL_LOAD_SEG    equ 0x1000        ; Where the kernel will be loaded initially @ 0x10000
    KERNEL_LOAD_OFFSET equ 0x0000        ; Where the kernel will be loaded initially
    KERNEL_LOAD_ADDR   equ ((KERNEL_LOAD_SEG << 4) + KERNEL_LOAD_OFFSET)

    KERNEL_START_ADDRESS equ 0x100000    ; Kernel's final memory address (1 MiB)


    ; Error messages
    disk_error_message db 'Disk Read Error!', 0
    vesa_error_message db 'VESA error!', 0


; ====================
; Real mode
; ====================

[BITS 16]


start:
    cli ; Dissabling interrupts

    ; Setting up segment registers and stack pointer
    xor ax, ax ; Zero AX register (this is faster than mov ax, 0)
    mov ds, ax
    mov es, ax
    mov ss, ax

    mov sp, 0x7000 ; Temporary stack pointer in real mode

    sti ; Enabling interrupts


    ; Checking hardware
    call check_hardware
    ; Getting usable RAM amount
    call get_ram_amount

    ; Enabling A20
    call init_a20

    ; Loading GDT
    lgdt [gdt_descriptor]



    ; Loading kernel using CHS
    mov bx, KERNEL_LOAD_OFFSET  ; Offset where kernel is loaded
    push KERNEL_LOAD_SEG        ; Segment where kernel is loaded
    pop es                      ; ES:BX=01000:0x0000 = phys address 0x10000

    xor dh, dh                  ; Head 0 (same as mov dh, 0)
    ; Use DL value passed by BIOS to our bootloader
    mov dl, 0x80
    mov cl, 0x02                ; Start reading from sector 2
    xor ch, ch                  ; Cylinder 0
    mov ah, 0x02                ; Function to read sectors
    mov al, __kernel_sectors    ; Number of sectors to read

    int 0x13                    ; BIOS interrupt to read sectors
    jc disk_read_error          ; If carry flag is set, handle disk read error


    ; Set VESA mode to 1024x768, 32bpp
    ; mov ax, 0x4F02      ; VBE function to set mode
    ; mov bx, 0x118 | 0x4000  ; 1024x768x32, enable linear frame buffer
    ; int 0x10            ; Call BIOS
    ; jc vesa_error



    ; Switching to protected mode
    cli             ; Dissabling interrupts

    mov eax, cr0
    or eax, 1       ; Set protected mode bit (PE) in CR0
    mov cr0, eax    ; Enable protected mode


    ; Perform a far jump to protected mode entry point
    jmp CODE_SEG_OFFSET:protected_mode_init


; ====================
; A20 line
; ====================

init_a20:
    push dx                  ; Save DX

    ; Wait for keyboard controller to be ready
    mov dx, 0x64
    in al, dx
    test al, 0x02            ; Check if bit 1 is set (input buffer full)
    jnz init_a20

    ; Send command to enable A20 line
    mov dx, 0x64
    mov al, 0xD1             ; Command to write next byte to output port
    out dx, al
    call wait_kbd_ready

    ; Write to output port to enable A20
    mov al, 0xDF             ; Enable A20 by clearing bit 1
    mov dx, 0x60
    out dx, al
    call wait_kbd_ready

    pop dx                  ; Restore DX

    ret

wait_kbd_ready:
    ; Wait until the keyboard controller is ready
    mov dx, 0x64  ; Set the port address to 0x64 (the status register of the keyboard controller)
    
wait_kbd_ready_loop:
    in al, dx     ; Read the status register from port 0x64 into AL
    test al, 0x02 ; Test if the 2nd bit of the status register is set (input buffer full)
    jnz wait_kbd_ready_loop ; If the bit is set (meaning the input buffer is full), loop until it is cleared

    ret           ; Once the input buffer is clear, return
    


; ====================
; Error codes
; ====================
disk_read_error:
    mov si, disk_error_message
    call print_str_realmode

    hlt
    jmp $

vesa_error:
    mov si, vesa_error_message
    call print_str_realmode

    hlt 
    jmp $


; ====================
; Protected mode
; ====================


[BITS 32] ; In protected mode

protected_mode_init:
    ; Set up segment registers in protected mode
    mov ax, DATA_SEG_OFFSET
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Set up stack pointer in protected mode
    mov ebp, 0x7C00
    mov esp, ebp


    ; Relocate the kernel to its final address (1 MiB)
    extern __kernel_start
    extern __kernel_load_end
    extern __kernel_load_sizeb

    mov esi, KERNEL_LOAD_ADDR      ; Source address
    mov edi, KERNEL_START_ADDRESS  ; Destination address (1 MiB)
    mov ecx, __kernel_load_sizeb   ; Set size in ECX for copying
    rep movsb                      ; Copy kernel to final address (EDI)

    ; Zero out the BSS memory
    extern __kernel_bss_start
    extern __kernel_bss_sizeb

    xor eax, eax                   ; AL = 0 (value to zero memory with)
    mov edi, __kernel_bss_start    ; Source address
    mov ecx, __kernel_bss_sizeb    ; Set size in ECX for copying
    rep stosb                      ; Zero fill the BBS memory

    ; Far jump to the kernel's entry point
    jmp CODE_SEG_OFFSET:_start

