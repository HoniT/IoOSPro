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

uint64_t* frame_bitmap = nullptr;

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
    frame_bitmap = reinterpret_cast<uint64_t*>(pmm::legacy_malloc(bitmap_size * sizeof(uint64_t)));

    // Error handling
    if (!frame_bitmap) {
        vga::error("Failed to allocate frame_bitmap!\n");
        return;
    }

    // Zero-initialize the bitmap
    memset(frame_bitmap, 0, bitmap_size);
    
    vga::printf("PMM initialized successfully!\n");
}


/* Returns true if free
 * Returns false if being used */
bool is_block_free(const uint32_t address) {
    return !(frame_bitmap[address / 64] & (uint64_t(1) << (address % 64)));
}

// Noting that the specific block has been allocated
void set_block_allocated(const uint32_t block_number) {
    // This performes a bitwise OR and modifies the lvalue
    frame_bitmap[block_number / 64] |= (uint64_t(1) << (block_number % 64));
}

// Noting that the specific block has been freed
void set_block_free(const uint32_t block_number) {
    // This performes a bitwise AND and modifies the lvalue
    frame_bitmap[block_number / 64] &= ~(uint64_t(1) << (block_number % 64)); 
}

#pragma endregion
#pragma region Block Handling

uint32_t pmm::allocate_frame() {
    // Itterating through all of the blocks untill we find an available one
    for(uint64_t i = 0; i < pmm::num_blocks; i++) {
        if(is_block_free(i)) {
            // Allocating and returning address
            set_block_allocated(i);

            return (i * BLOCK_SIZE + pmm::data_start_address);
        }
    }

    // Error: no more memory!
    vga::error("No more free memory to allocate frame!\n");
    return -1;
}

void pmm::free_frame(const uint32_t address) {
    // Converting the addres into a block index and setting it to allocated
    set_block_free((address - pmm::data_start_address) / BLOCK_SIZE);
}

void pmm::test_pmm() {
    // Block 1
    uint32_t block1 = allocate_frame();
    vga::printf("Block 1: ");
    vga::printf(block1);
    vga::printf('\n');

    // Block2
    uint32_t block2 = allocate_frame();
    vga::printf("Block 2: ");
    vga::printf(block2);
    vga::printf('\n');

    // Freeing Block 1
    free_frame(block1);

    // Allocating Block 3 after freeing
    uint32_t block3 = allocate_frame();
    vga::printf("Block 3: ");
    vga::printf(block3);
    vga::printf('\n');
}

#pragma endregion
