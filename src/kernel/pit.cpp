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
