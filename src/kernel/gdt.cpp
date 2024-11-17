// =======================================================================
// Copyright Ioane Baidoshvili 2024.
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
// http://www.opensource.org/licenses/MIT_1_0.txt)
//
// gdt.cpp sets up the GDT for in the kernel
// This file contains: 
// Global Descriptor Table that include TSS aswell
// =======================================================================

#include <gdt.hpp>
#include <utils/util.hpp>
#include <drivers/vga_print.hpp>

// Arrays and variables

gdt_entry gdt_entries[GDT_SEGMENT_QUANTITY];
gdt_ptr _gdt_ptr;
tss_entry _tss_entry;

void gdt::init() {
    // Set up GDT pointer
    _gdt_ptr.limit = (sizeof(struct gdt_entry) * 6) - 1;
    _gdt_ptr.base = (uint32_t)&gdt_entries; // Address of gdt_entries[0]

    // Setting up registers
    setGdtGate(0,0,0,0,0); //Null segment
    setGdtGate(1,0,0xFFFFFFFF, 0x9A, 0xCF); //Kernel code segment
    setGdtGate(2,0,0xFFFFFFFF, 0x92, 0xCF); //Kernel data segment
    setGdtGate(3,0,0xFFFFFFFF, 0xFA, 0xCF); //User code segment
    setGdtGate(4,0,0xFFFFFFFF, 0xF2, 0xCF); //User data segment
    writeTss(5,0x10, 0x0); // TSS

    // Loading GDT and TSS
    gdt_flush((uint32_t)&_gdt_ptr);
    vga::printf("Implemented GDT!\n");
    tss_flush();
    vga::printf("Implemented TSS!\n");
}

void gdt::setGdtGate(uint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran){
    // Setting up bases
    gdt_entries[num].base_low = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high = (base >> 24) & 0xFF;

    // Flags, limit and access flags
    gdt_entries[num].limit = (limit & 0xFFFF);
    gdt_entries[num].flags = (limit >> 16) & 0x0F;
    gdt_entries[num].flags |= (gran & 0xF0); // Granuality
    gdt_entries[num].access = access;

}

void gdt::writeTss(uint32_t num, uint16_t ss0, uint32_t esp0){
    uint32_t base = (uint32_t)&_tss_entry; // TSS address
    uint32_t limit = base + sizeof(_tss_entry);

    gdt::setGdtGate(num, base, limit, 0xE9, 0x00);
    memset(&_tss_entry, 0, sizeof(_tss_entry));

    _tss_entry.ss0 = ss0;
    _tss_entry.esp0 = esp0;

    _tss_entry.cs = 0x08 | 0x3;
    _tss_entry.ss = _tss_entry.ds = _tss_entry.es = _tss_entry.fs = _tss_entry.gs = 0x10 | 0x3;
}
