// =======================================================================
// Copyright Ioane Baidoshvili 2024.
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
// http://www.opensource.org/licenses/MIT_1_0.txt)
//
// idt_main.cpp is in charge of setting up the IDT
// This file contains: 
// Setting up IDT gates, ISR handlers, IRQ handlers, error messages ...
// =======================================================================

#include <idt.hpp>
#include <utils/util.hpp>
#include <utils/ports.hpp> 
#include <drivers/vga_print.hpp>

using ports::outPortB;

// ====================
// Structs and Functions
// ====================

struct idt_entry idt_entries[IDT_SIZE]; // 256 IDT entries in total
struct idt_ptr idt_ptr;

// Testing ISRs by dividing by zero
void idt::test_isr() {
    vga::printf((uint32_t)(1 / 0));
}


// Initializing IDT
void idt::init() {
    idt_ptr.limit = sizeof(struct idt_entry) * 256 - 1; // Same operation as gdt_descriptor in boot.asm
    idt_ptr.base = (uint32_t)&idt_entries;

    // Initialize everything to 0
    memset(&idt_entries, 0, sizeof(idt_entry) * 256);

    // Sending data to chips using outPortB defined in util.cpp
    // Configuring the Programmable Interrupt Controller (PIC)

    outPortB(0x20, 0x11);
    outPortB(0xA0, 0x11);
    outPortB(0x21, 0x20);
    outPortB(0xA1, 0x28);
    outPortB(0x21,0x04);
    outPortB(0xA1,0x02);
    outPortB(0x21, 0x01);
    outPortB(0xA1, 0x01);
    outPortB(0x21, 0x0);
    outPortB(0xA1, 0x0);

    // Setting every gate up
    // These all are 32 bit Interrupt Gates based off of the flag 0x8E

    setIdtGate(0, (uint32_t)isr0, 0x08, 0x8E);
    setIdtGate(1, (uint32_t)isr1, 0x08, 0x8E);
    setIdtGate(2, (uint32_t)isr2, 0x08, 0x8E);
    setIdtGate(3, (uint32_t)isr3, 0x08, 0x8E);
    setIdtGate(4, (uint32_t)isr4, 0x08, 0x8E);
    setIdtGate(5, (uint32_t)isr5, 0x08, 0x8E);
    setIdtGate(6, (uint32_t)isr6, 0x08, 0x8E);
    setIdtGate(7, (uint32_t)isr7, 0x08, 0x8E);
    setIdtGate(8, (uint32_t)isr8, 0x08, 0x8E);
    setIdtGate(9, (uint32_t)isr9, 0x08, 0x8E);
    setIdtGate(10, (uint32_t)isr10, 0x08, 0x8E);
    setIdtGate(11, (uint32_t)isr11, 0x08, 0x8E);
    setIdtGate(12, (uint32_t)isr12, 0x08, 0x8E);
    setIdtGate(13, (uint32_t)isr13, 0x08, 0x8E);
    setIdtGate(14, (uint32_t)isr14, 0x08, 0x8E);
    setIdtGate(15, (uint32_t)isr15, 0x08, 0x8E);
    setIdtGate(16, (uint32_t)isr16, 0x08, 0x8E);
    setIdtGate(17, (uint32_t)isr17, 0x08, 0x8E);
    setIdtGate(18, (uint32_t)isr18, 0x08, 0x8E);
    setIdtGate(19, (uint32_t)isr19, 0x08, 0x8E);
    setIdtGate(20, (uint32_t)isr20, 0x08, 0x8E);
    setIdtGate(21, (uint32_t)isr21, 0x08, 0x8E);
    setIdtGate(22, (uint32_t)isr22, 0x08, 0x8E);
    setIdtGate(23, (uint32_t)isr23, 0x08, 0x8E);
    setIdtGate(24, (uint32_t)isr24, 0x08, 0x8E);
    setIdtGate(25, (uint32_t)isr25, 0x08, 0x8E);
    setIdtGate(26, (uint32_t)isr26, 0x08, 0x8E);
    setIdtGate(27, (uint32_t)isr27, 0x08, 0x8E);
    setIdtGate(28, (uint32_t)isr28, 0x08, 0x8E);
    setIdtGate(29, (uint32_t)isr29, 0x08, 0x8E);
    setIdtGate(30, (uint32_t)isr30, 0x08, 0x8E);
    setIdtGate(31, (uint32_t)isr31, 0x08, 0x8E);

    setIdtGate(128, (uint32_t)isr128, 0x08, 0x8E); // System calls
    setIdtGate(177, (uint32_t)isr177, 0x08, 0x8E); // System calls


    setIdtGate(32, (uint32_t)irq0, 0x08, 0x8E);
    setIdtGate(33, (uint32_t)irq1, 0x08, 0x8E);
    setIdtGate(34, (uint32_t)irq2, 0x08, 0x8E);
    setIdtGate(35, (uint32_t)irq3, 0x08, 0x8E);
    setIdtGate(36, (uint32_t)irq4, 0x08, 0x8E);
    setIdtGate(37, (uint32_t)irq5, 0x08, 0x8E);
    setIdtGate(38, (uint32_t)irq6, 0x08, 0x8E);
    setIdtGate(39, (uint32_t)irq7, 0x08, 0x8E);
    setIdtGate(40, (uint32_t)irq8, 0x08, 0x8E);
    setIdtGate(41, (uint32_t)irq9, 0x08, 0x8E);
    setIdtGate(42, (uint32_t)irq10, 0x08, 0x8E);
    setIdtGate(43, (uint32_t)irq11, 0x08, 0x8E);
    setIdtGate(44, (uint32_t)irq12, 0x08, 0x8E);
    setIdtGate(45, (uint32_t)irq13, 0x08, 0x8E);
    setIdtGate(46, (uint32_t)irq14, 0x08, 0x8E);
    setIdtGate(47, (uint32_t)irq15, 0x08, 0x8E);


    idt_flush((uint32_t)&idt_ptr);
    vga::printf("Implemented IDT!\n");
}

// Sets an IDT gate
void idt::setIdtGate(const uint8_t num, const uint32_t base, const uint16_t selector, const uint8_t flags) {
    // Offsets
    idt_entries[num].offset_1 = base & 0xFFFF;
    idt_entries[num].offset_2 = (base >> 16) & 0xFFFF;

    idt_entries[num].selector = selector;
    idt_entries[num].zero = 0;
    idt_entries[num].gate_attributes = flags | 0x60;
}


// Exeption messages
const char* exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",
    "Double fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment not present",
    "Stack fault",
    "General protection fault",
    "Page fault",
    "Unknown Interrupt",
    "Coprocessor Fault",
    "Alignment Fault",
    "Machine Check", 
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};


// Interrupt Service Routine error message
extern "C" void isr_handler(struct InterruptRegisters* regs) {

    if(regs->interr_no < 32) {
        vga::print_set_color(PRINT_COLOR_WHITE, PRINT_COLOR_CYAN);
        vga::print_clear();

        vga::printf(exception_messages[regs->interr_no]);
        vga::printf("\nException! system Halted\n");

        asm volatile ("cli");
        while(true) { // Infinite loop
            // Halting CPU to stop further damage
            asm volatile ("hlt");
        }
        return;
    }
}


#pragma region IRQ Functions


// Array
void* irq_routines[IRQ_QUANTITY] = {
    // Zeroing all
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

// IRQ handler functions

extern "C" void irq_install_handler(int irq_num, void (*handler)(struct InterruptRegisters* regs)) {
    // Installing IRQ
    irq_routines[irq_num] = (void*)handler;
}

extern "C" void irq_uninstall_handler(int irq_num) {
    // Turrning IRQ back to 0
    irq_routines[irq_num] = 0;
}

// Interrupt request handler
extern "C" void irq_handler(struct InterruptRegisters* regs) {
    void (*handler)(struct InterruptRegisters* regs);

    // Getting value from irq_routines
    handler = (void (*)(struct InterruptRegisters*))irq_routines[regs->interr_no - 32];

    if(handler) {
        handler(regs);
    }

    // EOI signal
    if(regs->interr_no >= 40) {
        // Sending signal
        outPortB(0xA0, 0x20); // Slave PIC
    }

    outPortB(0x20, 0x20); // Master PIC
}

#pragma endregion