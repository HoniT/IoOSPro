// =======================================================================
// Copyright Ioane Baidoshvili 2024.
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
// http://www.opensource.org/licenses/MIT_1_0.txt)
// =======================================================================

#pragma once

#ifndef PIT_HPP
#define PIT_HPP

#include <stdint.h>

namespace pit {
    void init(); // Initializes the PIT
    void delay(uint64_t ms);
    
    void test();
} // Namespace pit

#endif // PIT_HPP