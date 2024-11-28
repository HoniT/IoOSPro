// =======================================================================
// Copyright Ioane Baidoshvili 2024.
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
// http://www.opensource.org/licenses/MIT_1_0.txt)
//
// malloc.cpp provide heap allocation and different memory management functions
// =======================================================================

#include <memory/physical/malloc.hpp>
#include <memory/physical/pmm.hpp>
#include <stdint.h>
#include <drivers/vga_print.hpp> // Assume you have a VGA print function for debugging

// Constants
constexpr size_t HEAP_START = 0x200000; // Example heap start (2 MB mark)
constexpr size_t HEAP_SIZE = 0x100000;  // 1 MB heap size

// Heap Allocator Variables
static char* heap_start = nullptr;   // Pointer to heap start
static size_t heap_offset = 0;       // Current heap offset
static size_t heap_size = 0;         // Total heap size

// Utility: Align Up
constexpr size_t pmm::align_up(size_t value, size_t alignment) {
    return (value + alignment - 1) & ~(alignment - 1);
}

// Initialize the Heap
void pmm::init_heap() {
    heap_start = reinterpret_cast<char*>(HEAP_START);
    heap_offset = 0;
    heap_size = HEAP_SIZE;

    vga::printf("Heap initialized!\n");
}

// Simple Malloc Implementation
void* pmm::legacy_malloc(size_t size) {
    size = align_up(size, 8); // Align to 8 bytes for safety

    if (!heap_start || heap_offset + size > heap_size) {
        vga::printf("Out of memory!\n");
        return nullptr; // Out of memory
    }

    void* block = heap_start + heap_offset;
    heap_offset += size;

    return block;
}
