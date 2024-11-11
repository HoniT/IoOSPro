// =======================================================================
// Copyright Ioane Baidoshvili 2024.
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
// http://www.opensource.org/licenses/MIT_1_0.txt)
// =======================================================================

#pragma once

#ifndef PORTS_HPP
#define PORTS_HPP

#include <stdint.h>

namespace ports{

// OutPortB and InPortB

// Sends a 8-bit value to a specified I/O port
void outPortB(const uint16_t Port, const uint8_t Value);

// Receives a 8-bit value from a specific I/O port
uint8_t inPortB(const uint16_t port);

} // Namespace ports

// Registers related to an interrupt
struct InterruptRegisters{
    uint32_t cr2;
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t interr_no, err_code;
    uint32_t eip, csm, eflags, useresp, ss;
};


#endif // PORTS_HPP
