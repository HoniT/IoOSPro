// =======================================================================
// Copyright Ioane Baidoshvili 2024.
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
// http://www.opensource.org/licenses/MIT_1_0.txt)
// =======================================================================

#pragma once

#ifndef GDT_HPP
#define GDT_HPP

#include <stdint.h>

#define GDT_SEGMENT_QUANTITY 6

// Functions

namespace gdt {
    void init(); // Initializes the GDT
    void setGdtGate(uint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran); // Sets GDT gate
    void writeTss(uint32_t num, uint16_t ss0, uint32_t esp0);
}

extern "C" void gdt_flush(uint32_t);
extern "C" void tss_flush();

// Structs

struct tss_entry {
    uint32_t link; // Contains the segment selector for the TSS of the previous task; 0x00; Reserved
    uint32_t esp0; // Stack pointer; 0x04
    uint32_t ss0; // Segment selector; 0x08; Reserved
    uint32_t esp1; // Stack pointer; 0x0C
    uint32_t ss1; // Segment selector; 0x10; Reserved
    uint32_t esp2; // Stack pointer; 0x14
    uint32_t ss2; // Segment selector; 0x18; Reserved
    uint32_t cr3, eip, eflags, eax, ecx, edx, ebx, esp, ebp, esi, edi; // 0x1C .. 0x44
    uint32_t es, cs, ss, ds, fs, gs, ldtr; // 0x48 .. 0x60
    uint32_t iopb; // I/O map base address; 0x64; Reserved
    uint32_t ssp; // Shadow stack pointer; 0x68
} __attribute__((packed));

// Needs to be 8 bytes exactly because we have a 32-bit OS
struct gdt_entry {
    uint16_t limit;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t flags;
    uint8_t base_high;
} __attribute__((packed));

struct gdt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

#endif // GDT_HPP