// =======================================================================
// Copyright Ioane Baidoshvili 2024.
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
// http://www.opensource.org/licenses/MIT_1_0.txt)
// =======================================================================

#pragma once

#ifndef IDT_MAIN_HPP
#define IDT_MAIN_HPP

#include <stdint.h>

#define IDT_SIZE 256
#define IRQ_QUANTITY 16

#pragma region Structs and Variables

// This ALWAYS NEEDS TO BE 8 BYTES IN TOTAL, because we have a 32 bit OS
struct idt_entry {
    uint16_t offset_1; // Offset bits 0-15
    uint16_t selector; // A code segment selector in the GDT
    uint8_t zero; // This is always zero. Unused
    uint8_t gate_attributes; // Gate type, DPL and P fields
    uint16_t offset_2; // Offset bits 16-31
} __attribute__((packed));

// Pointer struct
struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

#pragma endregion

#pragma region IDT Functions

namespace idt {

void init(); // Initializes IDT
void setIdtGate(const uint8_t num, const uint32_t base, const uint16_t selector, const uint8_t flags);
extern "C" void irq_install_handler(int irq_num, void (*handler)(struct InterruptRegisters* regs));
extern "C" void irq_uninstall_handler(int irq_num);

// This will frow an ISR. If it successfully throws an ISR that means it works
void test_isr();

extern const char* exception_messages[];

} // Namespace idt

extern "C" void isr_handler(struct InterruptRegisters* regs);
extern "C" void irq_handler(struct InterruptRegisters* regs);

extern "C" void idt_flush(uint32_t);


extern "C" {

// ====================
// Interrupt Service Routines
// ====================

    void isr0();
    void isr1();
    void isr2();
    void isr3();
    void isr4();
    void isr5();
    void isr6();
    void isr7();
    void isr8();
    void isr9();
    void isr10();
    void isr11();
    void isr12();
    void isr13();
    void isr14();
    void isr15();
    void isr16();
    void isr17();
    void isr18();
    void isr19();
    void isr20();
    void isr21();
    void isr22();
    void isr23();
    void isr24();
    void isr25();
    void isr26();
    void isr27();
    void isr28();
    void isr29();
    void isr30();
    void isr31();
    // Syscalls
    void isr128();
    void isr177();

// ====================
// Interrupt Service Routines
// ====================

    void irq0();
    void irq1();
    void irq2();
    void irq3();
    void irq4();
    void irq5();
    void irq6();
    void irq7();
    void irq8();
    void irq9();
    void irq10();
    void irq11();
    void irq12();
    void irq13();
    void irq14();
    void irq15();
}

#pragma endregion

#endif // IDT_MAIN_HPP
