// =======================================================================
// Copyright Ioane Baidoshvili 2024.
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
// http://www.opensource.org/licenses/MIT_1_0.txt)
//
// keyboard.cpp contains basic PS2 keyboard drivers
// =======================================================================

#include <drivers/keyboard.hpp>
#include <utils/ports.hpp>
#include <drivers/vga_print.hpp>
#include <idt.hpp>

using namespace ports;
using namespace keyboard;

bool capsOn, capsLock; // Two bool variables to manage lowercase and uppercase

void keyboardHandler(InterruptRegisters* regs) {
    // Getting scancode and press state
    uint8_t scancode = inPortB(KBD_DATA_PORT) & 0x7F;
    uint8_t press_state = inPortB(KBD_DATA_PORT) & 0x80; // Pressed down or released

    // Printing corresponding character
    switch(scancode) {
        // Special keycodes
        case 1:
        case 29:
        case 56:
        case 59:
        case 60:
        case 61:
        case 62:
        case 63:
        case 64:
        case 65:
        case 66:
        case 67:
        case 68:
        case 87:
        case 88:
            break;

        case 14:
            // Backspace
            if(press_state == 0)
                vga::backspace();
            break;

        case 42:
            // Shift key
            if (press_state == 0)
                capsOn = true;
            else
                capsOn = false;
            
            break;

        case 58:
            // Caps Lock key
            if (!capsLock && press_state == 0)
                capsLock = true;
            else if (capsLock && press_state == 0){
                capsLock = false;
            
            break;

        default:

            if (press_state == 0){
                if (capsOn || capsLock){
                    vga::printf((char)(uppercase[scancode]));
                }else{
                    vga::printf((char)(lowercase[scancode]));
                }
            }
        }
    }
}

void keyboard::init() {
    // Setting to lowercase originally
    capsOn = false; capsLock = false;

    // Setting up IRQ handler
    idt::irq_install_handler(1,&keyboardHandler);
    vga::printf("Initialized keyboard drivers!\n");
}