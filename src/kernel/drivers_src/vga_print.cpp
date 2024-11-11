// =======================================================================
// Copyright Ioane Baidoshvili 2024.
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
// http://www.opensource.org/licenses/MIT_1_0.txt)
//
// vga_print.cpp provides VGA printing functions before going into GFX mode
// This file contains: 
// printf function, cursor update function and more
// =======================================================================

#include <drivers/vga_print.hpp>
#include <utils/ports.hpp>

#pragma region Variables

// Size constraints
const size_t NUM_COLS = 80;
const size_t NUM_ROWS = 25;

// Defining structure of a character
struct Char {
    uint8_t character;
    uint8_t color;
};

// Current address/location
size_t col = 0;
size_t row = 0;

uint8_t color = PRINT_COLOR_WHITE | (PRINT_COLOR_BLACK << 4); // Standard white on black

bool printingString = false;

#pragma endregion


// ====================
// Local functions
// ====================

#pragma region Helper Functions

// Clears indicated line
void clear_row(const size_t row) {
    Char* buffer = reinterpret_cast<Char*>(VGA_ADDRESS);

    // Creating an empty struct
    Char empty {' ', color};

    // Iterating and clearing
    for(size_t col = 0; col < NUM_COLS; ++col) {
        buffer[col + NUM_COLS * row] = empty; // The newly created struct "empty"
    }
}

void print_newline() {
    Char* buffer = reinterpret_cast<Char*>(VGA_ADDRESS);

    col = 0;

    // Only adding new line if possible
    if(row < NUM_ROWS - 1) {
        ++row;
    } else {
        // Scrolling the screen up

        for(size_t r = 1; r < NUM_ROWS; ++r) {
            for(size_t c = 0; c < NUM_COLS; ++c) {
                // Copying this row to the one above
                buffer[c + NUM_COLS * (r - 1)] = buffer[c + NUM_COLS * r];
            }   
        }

        clear_row(NUM_ROWS - 1);
    }

}

// Convert a single nibble (4 bits) to its hex character representation
char nibble_to_hex(uint8_t nibble) {
    if (nibble < 10) {
        return '0' + nibble;
    } else {
        return 'A' + (nibble - 10);
    }
}

#ifdef PORTS_HPP

void update_cursor(const int row, const int col) {
    uint16_t pos = row * NUM_COLS + col;

    // Send the high byte of the cursor position
    ports::outPortB(0x3D4, 14);               // Select high cursor byte
    ports::outPortB(0x3D5, (pos >> 8) & 0xFF); // Send high byte

    // Send the low byte of the cursor position
    ports::outPortB(0x3D4, 15);               // Select low cursor byte
    ports::outPortB(0x3D5, pos & 0xFF);       // Send low byte
}

#endif // PORTS_HPP

#pragma endregion

#pragma region Print Functions

void print_char(const char character) {
    Char* buffer = reinterpret_cast<Char*>(VGA_ADDRESS);
    
    // Handeling new line character input
    if(character == '\n') {
        print_newline();

        return;
    }

    if(col >= NUM_COLS) {
        print_newline();
    }

    buffer[col + NUM_COLS * row] = {static_cast<uint8_t>(character), color};
    col++; // Incrementing character number on this line

    /* If its printing a string we will update the cursor at the end of the string 
    // for performance reasons */
    if(!printingString)
        update_cursor(row, col); 
}

// Printing string to the screen
void print_str(const char* str) {
    printingString = true;

    // Calling the print_char function the same amount of times as the strings length
    for(size_t i = 0; str[i] != '\0'; i++) {
        print_char(str[i]);
    }

    update_cursor(row, col); 
    printingString = false;
}

void print_hex(const uint32_t num) {
    print_str("0x"); // Prefix for hex numbers

    // Print each nibble (4 bits) as a hex digit
    for (int i = 7; i >= 0; i--) {
        uint8_t nibble = (num >> (i * 4)) & 0xF;  // Extract the current nibble
        print_char(nibble_to_hex(nibble));        // Print the corresponding hex digit
    }
}

#pragma endregion

// ====================
// Global functions
// ====================

namespace vga{

void init() {
    print_set_color(PRINT_COLOR_CYAN, PRINT_COLOR_BLUE);
    print_clear();
    
    printf(" ==================================== IoOS ==================================== \n");
}

// Print formatted overloads

void printf(const char print_object) {
    print_char(print_object);
}

void printf(const char* print_object) {
    print_str(print_object);
}

void printf(const uint32_t print_object) {
    print_hex(print_object);
}

// Error functions

void error(const char print_object) {
    // Changing foreground color to red
    print_set_color(PRINT_COLOR_RED, PRINT_COLOR_BLUE);
    print_char(print_object);

    // Halting CPU one time
    asm volatile ("hlt");
    print_set_color(PRINT_COLOR_CYAN, PRINT_COLOR_BLUE);
}

void error(const char* print_object) {
    // Changing foreground color to red
    print_set_color(PRINT_COLOR_RED, PRINT_COLOR_BLUE);
    print_str(print_object);

    // Halting CPU one time
    asm volatile ("hlt");
    print_set_color(PRINT_COLOR_CYAN, PRINT_COLOR_BLUE);
}

void error(const uint32_t print_object) {
    // Changing foreground color to red
    print_set_color(PRINT_COLOR_RED, PRINT_COLOR_BLUE);
    print_hex(print_object);

    // Halting CPU one time
    asm volatile ("hlt");
    print_set_color(PRINT_COLOR_CYAN, PRINT_COLOR_BLUE);
}


// Clears whole screen
void print_clear() {
    // Iterating through all of the rows, and using clear_row
    for(size_t row = 0; row < NUM_ROWS; ++row) {
        clear_row(row);
    }
}

// Backspace
void backspace() {
    Char* buffer = reinterpret_cast<Char*>(VGA_ADDRESS);

    if (row * NUM_COLS + col > 0) {
        // Decrementing column variable
        col--;
        // If it gets bellow 0
        if(col < 0) {
            row--;
            col = NUM_COLS;
        }

        // Replacing the standing character with a space and updating cursor
        buffer[col + NUM_COLS * row] = {static_cast<uint8_t>(' '), color};
        update_cursor(row, col);
    }
}

// Changing text color
void print_set_color(const uint8_t foreground, const uint8_t background) {
    color = foreground | (background << 4);
}

} // Namespace vga_print
