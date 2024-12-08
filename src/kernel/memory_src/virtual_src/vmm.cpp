// =======================================================================
// Copyright Ioane Baidoshvili 2024.
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
// http://www.opensource.org/licenses/MIT_1_0.txt)
//
// vmm.cpp sets up the VMM
// This file contains: 
// Managing virtual memory and addresses, paging...
// =======================================================================

#include <memory/virtual/vmm.hpp>
#include <memory/physical/pmm.hpp>
#include <utils/util.hpp>
#include <drivers/vga_print.hpp>

// Global variable for the kernel variable
PageDirectory* kernelPageDirectory;

// Map a virtual address to a physical address
void map_page(uint32_t virtualAddress, uint32_t physicalAddress, PageDirectory* directory, uint32_t flags) {
    uint32_t pageDirIndex = virtualAddress >> 22;         // Top 10 bits
    uint32_t pageTableIndex = (virtualAddress >> 12) & 0x3FF; // Middle 10 bits

    // Get or create the page table
    PageTable* pageTable;
    if (!(directory->entries[pageDirIndex].flags & PAGE_PRESENT)) {
        // Allocate a new page table
        uint32_t newTable = (uint32_t)pmm::allocate_frame(); // From PMM
        memset((void*)newTable, 0, PAGE_SIZE);
        directory->entries[pageDirIndex].address = newTable >> 12;
        directory->entries[pageDirIndex].flags = PAGE_PRESENT | PAGE_WRITABLE;
    }

    pageTable = (PageTable*)(directory->entries[pageDirIndex].address << 12);

    // Map the physical address to the virtual address
    pageTable->entries[pageTableIndex].address = physicalAddress >> 12;
    pageTable->entries[pageTableIndex].flags = flags;
}

void vmm::init() {
    // Allocate the kernel page directory
    kernelPageDirectory = (PageDirectory*)pmm::allocate_frame();
    memset(kernelPageDirectory, 0, PAGE_SIZE);

    // Identity map the kernel (map virtual == physical for now)
    for (uint32_t addr = 0; addr < 0x100000; addr += PAGE_SIZE) { // Map the first 1 MB
        map_page(addr, addr, kernelPageDirectory, PAGE_PRESENT | PAGE_WRITABLE);
    }

    // Load the page directory into CR3 and enable paging
    enable_paging(uint32_t(kernelPageDirectory));

    vga::printf("VMM initialized!\n");
}
