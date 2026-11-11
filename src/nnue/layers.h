#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include "nnue/network.h"

namespace athena::nnue {

template <std::size_t SIZE>
void add(int16_t* __restrict__ dst, 
   const int16_t* __restrict__ src) noexcept 
{
    for (std::size_t i = 0; i < SIZE; ++i)
        dst[i] += src[i];
}

template <std::size_t SIZE>
void sub(int16_t* __restrict__ dst, 
   const int16_t* __restrict__ src) noexcept
{
    for (std::size_t i = 0; i < SIZE; ++i)
        dst[i] -= src[i];
}

template <int32_t MIN = 0, int32_t MAX = 255>
inline int32_t screlu(int16_t x) noexcept {
    x = std::max(int16_t(MIN), x);
    x = std::min(int16_t(MAX), x);
    return int32_t(x) * int32_t(x);
}

template <std::size_t SIZE, int32_t MIN = 0, int32_t MAX = 255>
int32_t screlu_dot(const int16_t* __restrict__ acc,
                   const int16_t* __restrict__ w) noexcept
{
    int32_t sum = 0;
    for (std::size_t i = 0; i < SIZE; ++i)
        sum += screlu<MIN, MAX>(acc[i]) * int32_t(w[i]);
    return sum;
}

} // namespace athena
