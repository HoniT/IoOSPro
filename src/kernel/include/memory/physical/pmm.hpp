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

    void* block_malloc(); // Allocates a block of physical memory
    void block_free(const uint32_t address); // Frees a block of physical memory

    extern uint64_t usable_ram_amount;
    extern uint64_t num_blocks;

} // Namespace pmm

#endif // PMM_HPP
