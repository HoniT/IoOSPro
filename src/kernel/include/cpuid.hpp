// =======================================================================
// Copyright Ioane Baidoshvili 2024.
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
// http://www.opensource.org/licenses/MIT_1_0.txt)
// =======================================================================

#pragma once

#ifndef CPUID_HPP
#define CPUID_HPP

#include <stdint.h>

// CPUID info struct
struct cpuid_info_t {
    char vendorString[12];
    char nullTerminate = '\0';

    uint32_t features_ecx;
    uint32_t features_edx;
} __attribute__((packed));

#endif // CPUID_HPP