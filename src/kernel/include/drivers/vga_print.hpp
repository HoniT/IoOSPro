// =======================================================================
// Copyright Ioane Baidoshvili 2024.
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
// http://www.opensource.org/licenses/MIT_1_0.txt)
// =======================================================================

#pragma once

#ifndef VGA_PRINT_HPP
#define VGA_PRINT_HPP

#define VGA_ADDRESS 0xB8000 // VGA address

#include <stdint.h>
#include <stddef.h>

// 16 available colors with VGA
enum VGA_Print_Colors {

    PRINT_COLOR_BLACK = 0,
	PRINT_COLOR_BLUE = 1,
	PRINT_COLOR_GREEN = 2,
	PRINT_COLOR_CYAN = 3,
	PRINT_COLOR_RED = 4,
	PRINT_COLOR_MAGENTA = 5,
	PRINT_COLOR_BROWN = 6,
	PRINT_COLOR_LIGHT_GRAY = 7,
	PRINT_COLOR_DARK_GRAY = 8,
	PRINT_COLOR_LIGHT_BLUE = 9,
	PRINT_COLOR_LIGHT_GREEN = 10,
	PRINT_COLOR_LIGHT_CYAN = 11,
	PRINT_COLOR_LIGHT_RED = 12,
	PRINT_COLOR_PINK = 13,
	PRINT_COLOR_YELLOW = 14,
	PRINT_COLOR_WHITE = 15

};

namespace vga {

void init(); // Initializes VGA text

// VGA printing functions

// Print formatted
void printf(const char print_object);
void printf(const char* print_object);
void printf(const uint32_t print_object);
void printf(const uint64_t print_object);

// Error messages
void error(const char print_object);
void error(const char* print_object);
void error(const uint32_t print_object);

void print_clear(); // Clearing screen
void backspace();

void print_set_color(const uint8_t foreground, const uint8_t background); // Changing text color

} // Namespace vga

#endif // VGA_PRINT_HPP
