#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <cstdint>

using Byte = std::uint8_t;
using Word = std::uint16_t;
using u32  = std::uint32_t;

struct Mem {
    static constexpr u32 MAX_MEM = 1024 * 64;
    Byte Data[MAX_MEM];

    void Init() {
        for (u32 i = 0; i < MAX_MEM; i++)
        {
            Data[i] = 0;
        }
    }

    // read a byte
    Byte operator[](u32 Address) const {
        return Data[Address];
    }
    // write a byte
    Byte& operator[](u32 Address) {
        return Data[Address];
    }
};