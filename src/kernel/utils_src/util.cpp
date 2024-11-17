// =======================================================================
// Copyright Ioane Baidoshvili 2024.
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
// http://www.opensource.org/licenses/MIT_1_0.txt)
//
// util.cpp defines utility functions
// This file contains: 
// memset, strcmpr
// =======================================================================

#include <utils/util.hpp>

// Returns the difference in two strings
uint32_t strcmpr(const char* str1, const char* str2) {
    // Iterate over both strings and compare characters one by one
    while (*str1 && (*str1 == *str2)) {
        // Incrementing indexes
        str1++; str2++;
    }
    
    // Return the difference between the first non-matching characters
    return *(unsigned char*)str1 - *(unsigned char*)str2;
}

// Memset sets a block of memory to a specific value for a given number of bytes
void memset(const void *dest, const char val, uint32_t count){
    char *temp = (char*) dest;
    for (; count != 0; count--){
        *temp++ = val;
    }

}
