// =======================================================================
// Copyright Ioane Baidoshvili 2024.
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
// http://www.opensource.org/licenses/MIT_1_0.txt)
//
// kernel_main.cpp is the file that links every component of the kernel together
// This file contains: 
// Initializing different needed blocks of code, testing managers
// =======================================================================

#include <kernel_main.hpp>
#include <drivers/vga_print.hpp>
#include <drivers/keyboard.hpp>
#include <idt/idt.hpp>
#include <pit.hpp>
#include <gdt.hpp>
#include <memory/physical/pmm.hpp>
#include <memory/virtual/vmm.hpp>


extern "C" void kernel_main() {
    #pragma region Initialization

    vga::init(); // VGA text

    gdt::init(); // Global Descriptor Table
    idt::init(); // Interrupt Descriptor Table

    // Drivers
    pit::init(); // Programmable Interval Timer
    keyboard::init(); // PS2 keyboard drivers

    // Memory managers
    pmm::init();
    // vmm::init();

    #pragma endregion

    // Only uncomment if you want to test the ISR
    // idt::test_isr();

    #pragma region Testing

    pmm::test_pmm();
    pit::test();

    #pragma endregion
}
