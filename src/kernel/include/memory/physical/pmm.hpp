// =======================================================================
// Copyright Ioane Baidoshvili 2024.
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
// http://www.opensource.org/licenses/MIT_1_0.txt)
// =======================================================================

#pragma once

#ifndef PMM_HPP
#define PMM_HPP

#include <stdint.h>

#define BLOCK_SIZE 4096 // 4KiB
#define TOTAL_MEMORY


namespace pmm {
    void init(); // Initializes the PMM

    // Allocates a block of physical memory
    uint32_t allocate_frame();
    // Frees a block of physical memory
    void free_frame(const uint32_t address);
    // Testing the PMM allocation and deallocation functions
    void test_pmm();

    extern uint64_t usable_ram_amount;
    extern uint64_t num_blocks;

    // Start address of process data
    const uint32_t data_start_address = 0x500000;

} // Namespace pmm

#endif // PMM_HPP
