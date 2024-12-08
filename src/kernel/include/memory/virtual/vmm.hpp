// =======================================================================
// Copyright Ioane Baidoshvili 2024.
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
// http://www.opensource.org/licenses/MIT_1_0.txt)
// =======================================================================

#pragma once

#ifndef VMM_HPP
#define VMM_HPP

#define PAGE_SIZE 4096 // 4 KiB
#define PAGE_TABLE_ENTRIES 1024
#define PAGE_PRESENT 0X1
#define PAGE_WRITABLE 0X2
#define PAGE_USER 0X4

#include <stdint.h>

// Page Table entry
struct PageTableEntry {
    uint32_t address : 20; // 20 bit address pointing to a 4 KiB frame in physical memory
    uint32_t flags : 12; // 12 bits of flags e.g. read/write, present, user mode...
} __attribute__((packed));

// Page Table
struct PageTable {
    PageTableEntry entries[PAGE_TABLE_ENTRIES];
} __attribute__((packed));

// Page Directory entry
struct PageDirectoryEntry {
    uint32_t address : 20; // 20 bit address
    uint32_t flags : 12; // 12 bits of flags e.g. read/write, present, user mode...
} __attribute__((packed));

// Page Directory
struct PageDirectory {
    PageDirectoryEntry entries[PAGE_TABLE_ENTRIES];
} __attribute__((packed));

namespace vmm {
    void init();

} // namespace vmm

extern "C" void enable_paging(uint32_t);

#endif // VMM_HPP
