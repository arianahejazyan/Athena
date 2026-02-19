#pragma once

#include <immintrin.h>
#include "chess.h"

namespace athena
{

class alignas(CACHELINE_SIZE) Bitboard
{
    public:

    friend Bitboard subtract(Square sq1, Square sq2) noexcept;
    friend Bitboard rook_attacks(Square sq, const Bitboard& occupied) noexcept;

    // Default constructor
    constexpr Bitboard() = default;

    // Chunk-by-chunk constructor
    constexpr Bitboard(uint64_t chunk0, uint64_t chunk1, uint64_t chunk2, uint64_t chunk3) noexcept 
        : chunks_{chunk0, chunk1, chunk2, chunk3} {}

    // Chunk constructor
    explicit constexpr Bitboard(uint64_t chunk) noexcept 
        : chunks_{chunk, chunk, chunk, chunk} {}

    // Square constructor
    explicit constexpr Bitboard(Square sq) noexcept 
        : chunks_{0, 0, 0, 0}
    {
        set(sq);
    }

    // Copy constructor
    constexpr Bitboard(const Bitboard& other) noexcept 
        : chunks_{other.chunks_[0], other.chunks_[1], other.chunks_[2], other.chunks_[3]} {}

    // Move constructor
    constexpr Bitboard(Bitboard&& other) noexcept 
        : chunks_{other.chunks_[0], other.chunks_[1], other.chunks_[2], other.chunks_[3]} {}

    constexpr Bitboard operator=(const Bitboard& other) noexcept
    {
        for (std::size_t i = 0; i < CHUNK_NB; i++) {
            chunks_[i] = other.chunks_[i];
        }
        return *this;
    }

    // constexpr Bitboard(const Bitboard& other) noexcept
    //     : chunks_{other.chunks_[0], other.chunks_[1], other.chunks_[2], other.chunks_[3]} {}

    // constexpr Bitboard(uint64_t chunk0, uint64_t chunk1, uint64_t chunk2, uint64_t chunk3) noexcept
    //     : chunks_{chunk0, chunk1, chunk2, chunk3} {}

    constexpr Bitboard& operator&=(uint64_t mask) noexcept
    {
        for (std::size_t i = 0; i < CHUNK_NB; i++) {
            chunks_[i] &= mask;
        }
        return *this;
    }

    constexpr Bitboard& operator|=(uint64_t mask) noexcept
    {
        for (std::size_t i = 0; i < CHUNK_NB; i++) {
            chunks_[i] |= mask;
        }
        return *this;
    }

    Bitboard operator~() const noexcept
    {
        return Bitboard(
            ~chunks_[0], 
            ~chunks_[1], 
            ~chunks_[2], 
            ~chunks_[3]);
    }

    Bitboard operator&(const Bitboard& other) const noexcept {
        Bitboard result(*this);
        result &= other;
        return result;
    }

    Bitboard operator|(const Bitboard& other) const noexcept {
        Bitboard result(*this);
        result |= other;
        return result;
    }

    Bitboard operator&(uint64_t mask) const noexcept {
        Bitboard result(*this);
        result &= mask;
        return result;
    }

    Bitboard operator|(uint64_t mask) const noexcept {
        Bitboard result(*this);
        result |= mask;
        return result;
    }

    Bitboard& operator&=(const Bitboard& other) noexcept
    {
        for (std::size_t i = 0; i < CHUNK_NB; i++) {
            chunks_[i] &= other.chunks_[i];
        }
        return *this;
    }

    Bitboard& operator|=(const Bitboard& other) noexcept
    {
        for (std::size_t i = 0; i < CHUNK_NB; i++) {
            chunks_[i] |= other.chunks_[i];
        }
        return *this;
    }

    // Shift each 64-bit chunk independently (no carry between chunks)
    void operator<<(int s) noexcept;
    void operator>>(int s) noexcept;

    // Shift each 64-bit chunk dependently (cross-chunk carry)
    template<int offset>
    void shift() noexcept;

    // Set the bit at the given square
    constexpr void set(Square sq) noexcept;

    // Clear the bit at the given square
    constexpr void pop(Square sq) noexcept;

    // Check if the bit at the given square is set
    constexpr bool has(Square sq) const noexcept;

    // Combine the 4 chunks into a single 64-bit integer
    constexpr uint64_t combine() const noexcept;

    // Check if all bits are zero
    constexpr bool empty() const noexcept;

    // Count the number of set bits
    int count() const noexcept;

    // hex
    // decimal

    void print(bool debug = true) const;

    private:
    uint64_t chunks_[CHUNK_NB];
};

constexpr void Bitboard::set(Square sq) noexcept {
    chunks_[sq.chunk()] |= 1ULL << sq.index();
}

constexpr void Bitboard::pop(Square sq) noexcept {
    chunks_[sq.chunk()] &= ~(1ULL << sq.index());
}

constexpr bool Bitboard::has(Square sq) const noexcept {
    return (chunks_[sq.chunk()] >> sq.index()) & 0x1;
}

inline void Bitboard::operator<<(int s) noexcept
{
    chunks_[0] <<= s;
    chunks_[1] <<= s;
    chunks_[2] <<= s;
    chunks_[3] <<= s;
}

inline void Bitboard::operator>>(int s) noexcept
{
    chunks_[0] >>= s;
    chunks_[1] >>= s;
    chunks_[2] >>= s;
    chunks_[3] >>= s;
}

constexpr uint64_t Bitboard::combine() const noexcept
{
    return 
    chunks_[0] | 
    chunks_[1] | 
    chunks_[2] |
    chunks_[3];
}

constexpr bool Bitboard::empty() const noexcept {
    return combine() == 0;
}

inline int Bitboard::count() const noexcept
{
    return 
    __builtin_popcountll(chunks_[0]) +
    __builtin_popcountll(chunks_[1]) +
    __builtin_popcountll(chunks_[2]) +
    __builtin_popcountll(chunks_[3]);
}

// Return the bitboard of all bits between two squares
inline Bitboard subtract(Square sq1, Square sq2) noexcept
{
    if (sq1 < sq2)
        std::swap(sq1, sq2);

    Bitboard result;
    Bitboard bb1(sq1);
    Bitboard bb2(sq2);

    uint64_t borrow;
    borrow = __builtin_sub_overflow(bb1.chunks_[0], bb2.chunks_[0]         , &result.chunks_[0]);
    borrow = __builtin_sub_overflow(bb1.chunks_[1], bb2.chunks_[1] + borrow, &result.chunks_[1]);
    borrow = __builtin_sub_overflow(bb1.chunks_[2], bb2.chunks_[2] + borrow, &result.chunks_[2]);
             __builtin_sub_overflow(bb1.chunks_[3], bb2.chunks_[3] + borrow, &result.chunks_[3]);

    return result;
}

// ===== shift (template specialization) ===== //

template<>
inline void Bitboard::shift<Offset::North>() noexcept
{
#if __AVX2__
    __m256i b     = _mm256_load_si256(reinterpret_cast<const __m256i*>(chunks_));
    __m256i left  = _mm256_slli_epi64(b, 16);
    __m256i carry = _mm256_srli_epi64(b, 48);
    __m256i perm  = _mm256_permute4x64_epi64(carry, _MM_SHUFFLE(2,1,0,0));
    __m256i mask  = _mm256_set_epi64x(-1LL,-1LL,-1LL,0LL);
    _mm256_store_si256(reinterpret_cast<__m256i*>(chunks_), _mm256_or_si256(left, _mm256_and_si256(perm, mask)));
#else
    const uint64_t c0 = chunks_[0];
    const uint64_t c1 = chunks_[1];
    const uint64_t c2 = chunks_[2];
    const uint64_t c3 = chunks_[3];
    chunks_[0] =  c0 << 16;
    chunks_[1] = (c1 << 16) | (c0 >> 48);
    chunks_[2] = (c2 << 16) | (c1 >> 48);
    chunks_[3] = (c3 << 16) | (c2 >> 48);
#endif
}

template<>
inline void Bitboard::shift<Offset::South>() noexcept
{
#if __AVX2__
    __m256i b     = _mm256_load_si256(reinterpret_cast<const __m256i*>(chunks_));
    __m256i right = _mm256_srli_epi64(b, 16);
    __m256i carry = _mm256_slli_epi64(b, 48);
    __m256i perm  = _mm256_permute4x64_epi64(carry, _MM_SHUFFLE(0,3,2,1));
    __m256i mask  = _mm256_set_epi64x(0LL,-1LL,-1LL,-1LL);
    _mm256_store_si256(reinterpret_cast<__m256i*>(chunks_), _mm256_or_si256(right, _mm256_and_si256(perm, mask)));
#else
    const uint64_t c0 = chunks_[0];
    const uint64_t c1 = chunks_[1];
    const uint64_t c2 = chunks_[2];
    const uint64_t c3 = chunks_[3];
    chunks_[0] = (c0 >> 16) | (c1 << 48);
    chunks_[1] = (c1 >> 16) | (c2 << 48);
    chunks_[2] = (c2 >> 16) | (c3 << 48);
    chunks_[3] =  c3 >> 16;
#endif
}

template<>
inline void Bitboard::shift<Offset::East>() noexcept
{
#if __AVX2__
    __m256i b = _mm256_load_si256(reinterpret_cast<const __m256i*>(chunks_));
    _mm256_store_si256(reinterpret_cast<__m256i*>(chunks_), _mm256_slli_epi64(b,1));
#else
    chunks_[0] >>= 1;
    chunks_[1] >>= 1;
    chunks_[2] >>= 1;
    chunks_[3] >>= 1;
#endif
}

template<>
inline void Bitboard::shift<Offset::West>() noexcept
{
#if __AVX2__
    __m256i b = _mm256_load_si256(reinterpret_cast<const __m256i*>(chunks_));
    _mm256_store_si256(reinterpret_cast<__m256i*>(chunks_), _mm256_srli_epi64(b,1));
#else
    chunks_[0] <<= 1;
    chunks_[1] <<= 1;
    chunks_[2] <<= 1;
    chunks_[3] <<= 1;
#endif
}

template<>
inline void Bitboard::shift<Offset::North + Offset::East>() noexcept
{
    shift<Offset::North>();
    shift<Offset::East>();
}

template<>
inline void Bitboard::shift<Offset::North + Offset::West>() noexcept
{
    shift<Offset::North>();
    shift<Offset::West>();
}

template<>
inline void Bitboard::shift<Offset::South + Offset::East>() noexcept
{
    shift<Offset::South>();
    shift<Offset::East>();
}

template<>
inline void Bitboard::shift<Offset::South + Offset::West>() noexcept
{
    shift<Offset::South>();
    shift<Offset::West>();
}

} // namespace athena