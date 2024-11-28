// =======================================================================
// Copyright Ioane Baidoshvili 2024.
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
// http://www.opensource.org/licenses/MIT_1_0.txt)
//
// pmm.cpp sets up the PMM
// This file contains: 
// Memory bitmaps, allocating and freeing blocks of physical memory...
// =======================================================================

#include <memory/physical/pmm.hpp>
#include <memory/physical/malloc.hpp>
#include <drivers/vga_print.hpp>
#include <utils/util.hpp>


uint64_t pmm::usable_ram_amount = 0; // Total usable RAM
uint64_t pmm::num_blocks = 0; // Total amount of blocks for the PMM
size_t bitmap_size;

uint64_t* mem_bitmap = nullptr;

#pragma region Initialization

void iterate_mmap_entries(uint16_t* mmap_entries) {
    uint16_t entry_count = *mmap_entries;

    uint32_t* entry_ptr = (uint32_t*)mmap_entries + 1; // This will give us 0x8004, which is the address where the mmap entries start

    // Iterating through the mmap entries
    for(uint16_t i = 0; i < entry_count; i++) {
        // This gives us the address of the cuurent entry
        uint8_t* entry = reinterpret_cast<uint8_t*>(entry_ptr) + i * 24;

        // The get_mmap function in hardware_init.asm only gets usable memory map entries
        // So we won't need to check if there usable here

        uint32_t lower_length = *reinterpret_cast<uint32_t*>(entry + 8);  // Lower 32 bits
        uint32_t upper_length = *reinterpret_cast<uint32_t*>(entry + 12); // Upper 32 bits
        uint64_t total_length = (uint64_t) upper_length << 32 | lower_length; // Combine them

        pmm::usable_ram_amount += total_length;
    }
}

void pmm::init() {
    init_heap();

    // Getting total amount of usable memory
    uint16_t* mmap_ent = reinterpret_cast<uint16_t*>(0x8000); // Assume memory map at 0x8000
    iterate_mmap_entries(mmap_ent);

    // Calculate number of blocks and allocate the bitmap
    pmm::num_blocks = pmm::usable_ram_amount / BLOCK_SIZE;
    bitmap_size = (pmm::num_blocks + 63) / 64; // Number of uint64_t elements needed
    mem_bitmap = reinterpret_cast<uint64_t*>(pmm::legacy_malloc(bitmap_size * sizeof(uint64_t)));

    // Error handling
    if (!mem_bitmap) {
        vga::error("Failed to allocate mem_bitmap!\n");
        return;
    }

    // Zero-initialize the bitmap
    for (size_t i = 0; i < bitmap_size; i++) {
        mem_bitmap[i] = 0;
    }

    vga::printf("PMM initialized successfully!\n");
}

#pragma endregion
#pragma region Block Handling

void* pmm::block_malloc() {

}

void pmm::block_free(const uint32_t address) {

}

#pragma endregion
