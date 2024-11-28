// =======================================================================
// Copyright Ioane Baidoshvili 2024.
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
// http://www.opensource.org/licenses/MIT_1_0.txt)
//
// kernel_panic.cpp is in charge of handling kernel panics
// This file contains: 
// Handling kernel panics, restarting system after kernel panic
// =======================================================================

#include <idt/kernel_panic.hpp>
#include <idt/idt.hpp>
#include <drivers/vga_print.hpp>
#include <utils/ports.hpp>

void kernel_panic(const uint32_t error_index) {
    asm volatile ("cli"); // Clearing interrupts
    
    vga::print_set_color(PRINT_COLOR_WHITE, PRINT_COLOR_CYAN);
    vga::print_clear();

    vga::printf(idt::exception_messages[error_index]);
    vga::printf("\nException! system Halted\n");

    // Will save dump in the future, instead of infinite loop
    for(;;) {}

    ports::outPortB(0x64, 0xFE); // Restarting

    return;
}
