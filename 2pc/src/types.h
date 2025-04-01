#ifndef TYPES_H
#define TYPES_H

#include <array>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace Talia
{

using BitBoard  = uint64_t;

using u64 = uint64_t;
using u32 = uint32_t;
using u16 = uint16_t;
using u8  = uint8_t;

template<typename T, std::size_t... Dimensions>
struct MultiDimArray;

template <typename T, std::size_t Dim>
struct MultiDimArray<T, Dim> {
    using type = std::array<T, Dim>;
};

template <typename T, std::size_t FirstDim, std::size_t... RemainingDims>
struct MultiDimArray<T, FirstDim, RemainingDims...> {
    using type = std::array<typename MultiDimArray<T, RemainingDims...>::type, FirstDim>;
};

template <typename T, std::size_t... Dimensions>
using StaticArray = typename MultiDimArray<T, Dimensions...>::type;

template <typename T, std::size_t... Dimensions>
using MDA = typename MultiDimArray<T, Dimensions...>::type;

template<typename T>
using DynamicArray = std::vector<T>;

} // namespace

#endif