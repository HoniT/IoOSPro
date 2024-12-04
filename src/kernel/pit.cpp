// =======================================================================
// Copyright Ioane Baidoshvili 2024.
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
// http://www.opensource.org/licenses/MIT_1_0.txt)
//
// pit.cpp initializes the Programmable Interval Timer
// =======================================================================

#include <pit.hpp>
#include <utils/ports.hpp>
#include <idt/idt.hpp>
#include <drivers/vga_print.hpp>

volatile uint64_t ticks;
const uint32_t frequency = 100;

// PIT is IRQ0
void onIrq0(InterruptRegisters* regs) {
    ticks++;
    ports::outPortB(0x20, 0x20);  // Send EOI to PIC (if necessary)
}

void pit::init() {
    ticks = 0; // Zeroing ticks
    // Installing handler
    idt::irq_install_handler(0, &onIrq0);

    // 119318.16666 Mhz
    uint32_t divisor = 1193180 / frequency;

    ports::outPortB(0x43,0x36);
    ports::outPortB(0x40,(uint8_t)(divisor & 0xFF));
    ports::outPortB(0x40,(uint8_t)((divisor >> 8) & 0xFF));

}

void pit::delay(uint64_t ms) {
    // A simple delay that waits for `ms` milliseconds (approximated)
    uint32_t start = ticks;
    uint64_t targetTicks = start + ms;  // Target ticks to achieve the delay

    // Wait until the target number of ticks has passed
    while (ticks < targetTicks) {
        // You could add a small NOP here if you want to reduce CPU usage slightly
        __asm__("nop");
    }
}

void pit::test() {
    vga::printf("Starting timer\n");
    pit::delay(300);
    vga::printf("3 seconds have passed!\n");
}
