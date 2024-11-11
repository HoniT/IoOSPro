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

// Functions defined in util.cpp
void memset(const void *dest, const char val, uint32_t count);
uint32_t strcmpr(const char* str1, const char* str2);

#endif // UTIL_HPP
