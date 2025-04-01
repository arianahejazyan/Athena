// #ifndef RANDOM_H
// #define RANDOM_H

// #include "types.h"

// namespace Talia
// {

// constexpr u64 seed = 6093652183137414463;

// [[nodiscard]] consteval u64 xorshift64(u64 seed) noexcept
// {
//     seed ^= seed >> 12;
//     seed ^= seed << 25;
//     seed ^= seed >> 27;

//     return seed * 2685821657736338717ull;
// }

// } // namespace

// #endif