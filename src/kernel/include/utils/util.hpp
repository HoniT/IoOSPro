// =======================================================================
// Copyright Ioane Baidoshvili 2024.
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
// http://www.opensource.org/licenses/MIT_1_0.txt)
// =======================================================================

#pragma once

#ifndef UTIL_HPP
#define UTIL_HPP

#include <stdint.h>
#include <stddef.h>

// Functions defined in util.cpp
void memset(const void *dest, const char val, uint32_t count);
namespace pmm { constexpr size_t align_up(size_t value, size_t alignment); }

#endif // UTIL_HPP
