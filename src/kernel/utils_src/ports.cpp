// =======================================================================
// Copyright Ioane Baidoshvili 2024.
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
// http://www.opensource.org/licenses/MIT_1_0.txt)
//
// ports.cpp defines inB and outB I/O functions
// This file contains: 
// inPortB and outPortB
// =======================================================================

#include <utils/ports.hpp>

namespace ports{

// OutPortB and InPortB

// Sends a 8-bit value to a specified I/O port
void outPortB(const uint16_t Port, const uint8_t Value) {
    asm volatile ("outb %1, %0" : : "dN" (Port), "a" (Value));
}

// Receives a 8-bit value from a specific I/O port
uint8_t inPortB(const uint16_t port) {
    uint8_t value;

    asm volatile("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

} // Namespace ports