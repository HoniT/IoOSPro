// =======================================================================
// Copyright Ioane Baidoshvili 2024.
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
// http://www.opensource.org/licenses/MIT_1_0.txt)
// =======================================================================

#pragma once

#ifndef MALLOC_HPP
#define MALLOC_HPP

#include <stddef.h>
#include <stdint.h>

namespace pmm {
// Heap allocation
uint32_t legacy_malloc(size_t size);

void init_heap();

} // namespace pmm

#endif // MALLOC_HPP
