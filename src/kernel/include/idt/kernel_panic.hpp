// =======================================================================
// Copyright Ioane Baidoshvili 2024.
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
// http://www.opensource.org/licenses/MIT_1_0.txt)
// =======================================================================

#pragma once

#ifndef KERNEL_PANIC_HPP
#define KERNEL_PANIC_HPP

#include <stdint.h>

void kernel_panic(const uint32_t error_index);

#endif // KERNEL_PANIC_HPP
