#pragma once

#include <immintrin.h>
#include "chess.h"

namespace athena
{

class alignas(CACHELINE_SIZE) Bitboard
{
    public:
    Bitboard() = default;

    constexpr Bitboard operator=(const Bitboard& other) noexcept
    {
        for (std::size_t i = 0; i < CHUNK_NB; i++) {
            chunks_[i] = other.chunks_[i];
        }
        return *this;
    }

    constexpr Bitboard(const Bitboard& other) noexcept
        : chunks_{other.chunks_[0], other.chunks_[1], other.chunks_[2], other.chunks_[3]} {}

    constexpr Bitboard(uint64_t chunk0, uint64_t chunk1, uint64_t chunk2, uint64_t chunk3) noexcept
        : chunks_{chunk0, chunk1, chunk2, chunk3} {}

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

    Bitboard operator<<(int s) const noexcept;
    Bitboard operator>>(int s) const noexcept;

    template<Offset offset>
    Bitboard shift() noexcept
    {
        Bitboard result;

        #if __AVX2__
        __m256i b = _mm256_load_si256(reinterpret_cast<const __m256i*>(chunks_));
        __m256i r;
    
        if constexpr (offset == Offset::E)
        {
            r = _mm256_srli_epi64(b, 1);
        }
        else if constexpr (offset == Offset::W)
        {
            r = _mm256_slli_epi64(b, 1);
        }
        else if constexpr (offset == Offset::N)
        {
            __m256i left = _mm256_slli_epi64(b, 16);
            __m256i carry = _mm256_srli_epi64(b, 48);
            __m256i perm = _mm256_permute4x64_epi64(carry, _MM_SHUFFLE(2, 1, 0, 0));
            __m256i mask = _mm256_set_epi64x(-1LL, -1LL, -1LL, 0LL);
            __m256i carry_shifted = _mm256_and_si256(perm, mask);
            r = _mm256_or_si256(left, carry_shifted);
        }
        else if constexpr (offset == Offset::S)
        {
            __m256i right = _mm256_srli_epi64(b, 16);
            __m256i carry = _mm256_slli_epi64(b, 48);
            __m256i perm = _mm256_permute4x64_epi64(carry, _MM_SHUFFLE(0, 3, 2, 1));
            __m256i mask = _mm256_set_epi64x(0LL, -1LL, -1LL, -1LL);
            __m256i carry_shifted = _mm256_and_si256(perm, mask);
            r = _mm256_or_si256(right, carry_shifted);
        }
        else if constexpr (offset == Offset::N + Offset::E)
        {
            __m256i left = _mm256_slli_epi64(b, 16);
            __m256i carry = _mm256_srli_epi64(b, 48);
            __m256i perm = _mm256_permute4x64_epi64(carry, _MM_SHUFFLE(2, 1, 0, 0));
            __m256i mask = _mm256_set_epi64x(-1LL, -1LL, -1LL, 0LL);
            __m256i carry_shifted = _mm256_and_si256(perm, mask);
            __m256i north = _mm256_or_si256(left, carry_shifted);
            r = _mm256_srli_epi64(north, 1);
        }
        else if constexpr (offset == Offset::N + Offset::W)
        {
            __m256i left = _mm256_slli_epi64(b, 16);
            __m256i carry = _mm256_srli_epi64(b, 48);
            __m256i perm = _mm256_permute4x64_epi64(carry, _MM_SHUFFLE(2, 1, 0, 0));
            __m256i mask = _mm256_set_epi64x(-1LL, -1LL, -1LL, 0LL);
            __m256i carry_shifted = _mm256_and_si256(perm, mask);
            __m256i north = _mm256_or_si256(left, carry_shifted);
            r = _mm256_slli_epi64(north, 1);
        }
        else if constexpr (offset == Offset::S + Offset::E)
        {
            __m256i right = _mm256_srli_epi64(b, 16);
            __m256i carry = _mm256_slli_epi64(b, 48);
            __m256i perm = _mm256_permute4x64_epi64(carry, _MM_SHUFFLE(0, 3, 2, 1));
            __m256i mask = _mm256_set_epi64x(0LL, -1LL, -1LL, -1LL);
            __m256i carry_shifted = _mm256_and_si256(perm, mask);
            __m256i south = _mm256_or_si256(right, carry_shifted);
            r = _mm256_srli_epi64(south, 1);
        }
        else if constexpr (offset == Offset::S + Offset::W)
        {
            __m256i right = _mm256_srli_epi64(b, 16);
            __m256i carry = _mm256_slli_epi64(b, 48);
            __m256i perm = _mm256_permute4x64_epi64(carry, _MM_SHUFFLE(0, 3, 2, 1));
            __m256i mask = _mm256_set_epi64x(0LL, -1LL, -1LL, -1LL);
            __m256i carry_shifted = _mm256_and_si256(perm, mask);
            __m256i south = _mm256_or_si256(right, carry_shifted);
            r = _mm256_slli_epi64(south, 1);
        }
    
        _mm256_store_si256(reinterpret_cast<__m256i*>(result.chunks_), r);
        #else
        if constexpr (offset == Offset::E)
        {
            result.chunks_[0] = chunks_[0] >> 1;
            result.chunks_[1] = chunks_[1] >> 1;
            result.chunks_[2] = chunks_[2] >> 1;
            result.chunks_[3] = chunks_[3] >> 1;
        }
        else if constexpr (offset == Offset::W)
        {
            result.chunks_[0] = chunks_[0] << 1;
            result.chunks_[1] = chunks_[1] << 1;
            result.chunks_[2] = chunks_[2] << 1;
            result.chunks_[3] = chunks_[3] << 1;
        }
        else if constexpr (offset == Offset::N)
        {
            const uint64_t c0 = chunks_[0];
            const uint64_t c1 = chunks_[1];
            const uint64_t c2 = chunks_[2];
            const uint64_t c3 = chunks_[3];
            
            result.chunks_[0] =  c0 << 16;
            result.chunks_[1] = (c1 << 16) | (c0 >> 48);
            result.chunks_[2] = (c2 << 16) | (c1 >> 48);
            result.chunks_[3] = (c3 << 16) | (c2 >> 48);
        }
        else if constexpr (offset == Offset::S) 
        {
            const uint64_t c0 = chunks_[0];
            const uint64_t c1 = chunks_[1];
            const uint64_t c2 = chunks_[2];
            const uint64_t c3 = chunks_[3];
            
            result.chunks_[0] = (c0 >> 16) | (c1 << 48);
            result.chunks_[1] = (c1 >> 16) | (c2 << 48);
            result.chunks_[2] = (c2 >> 16) | (c3 << 48);
            result.chunks_[3] =  c3 >> 16;
        }
        else if constexpr (offset == Offset::N + Offset::E) 
        {
            const uint64_t c0 = chunks_[0];
            const uint64_t c1 = chunks_[1];
            const uint64_t c2 = chunks_[2];
            const uint64_t c3 = chunks_[3];
            
            result.chunks_[0] =  (c0 << 16) >> 1;
            result.chunks_[1] = ((c1 << 16) | (c0 >> 48)) >> 1;
            result.chunks_[2] = ((c2 << 16) | (c1 >> 48)) >> 1;
            result.chunks_[3] = ((c3 << 16) | (c2 >> 48)) >> 1;
        }
        else if constexpr (offset == Offset::N + Offset::W) 
        {
            const uint64_t c0 = chunks_[0];
            const uint64_t c1 = chunks_[1];
            const uint64_t c2 = chunks_[2];
            const uint64_t c3 = chunks_[3];
            
            result.chunks_[0] =  (c0 << 16) << 1;
            result.chunks_[1] = ((c1 << 16) | (c0 >> 48)) << 1;
            result.chunks_[2] = ((c2 << 16) | (c1 >> 48)) << 1;
            result.chunks_[3] = ((c3 << 16) | (c2 >> 48)) << 1;
        }
        else if constexpr (offset == Offset::S + Offset::E) 
        {
            const uint64_t c0 = chunks_[0];
            const uint64_t c1 = chunks_[1];
            const uint64_t c2 = chunks_[2];
            const uint64_t c3 = chunks_[3];
            
            result.chunks_[0] = ((c0 >> 16) | (c1 << 48)) >> 1;
            result.chunks_[1] = ((c1 >> 16) | (c2 << 48)) >> 1;
            result.chunks_[2] = ((c2 >> 16) | (c3 << 48)) >> 1;
            result.chunks_[3] =  (c3 >> 16) >> 1;
        }
        else if constexpr (offset == Offset::S + Offset::W) 
        {
            const uint64_t c0 = chunks_[0];
            const uint64_t c1 = chunks_[1];
            const uint64_t c2 = chunks_[2];
            const uint64_t c3 = chunks_[3];
            
            result.chunks_[0] = ((c0 >> 16) | (c1 << 48)) << 1;
            result.chunks_[1] = ((c1 >> 16) | (c2 << 48)) << 1;
            result.chunks_[2] = ((c2 >> 16) | (c3 << 48)) << 1;
            result.chunks_[3] =  (c3 >> 16) << 1;
        }
        #endif

        return result;
    }

    constexpr void setSquare(Square sq) noexcept {
        chunks_[chunk(sq)] |= 1ULL << index(sq);
    }

    constexpr void popSquare(Square sq) noexcept {
        chunks_[chunk(sq)] &= ~(1ULL << index(sq));
    }

    constexpr bool hasSquare(Square sq) const noexcept {
        return (chunks_[chunk(sq)] >> index(sq)) & 0x1;
    }

    constexpr uint64_t combine() const noexcept
    {
        return 
        chunks_[0] | 
        chunks_[1] | 
        chunks_[2] |
        chunks_[3];
    }

    constexpr bool empty() const noexcept
    {
        return 
        chunks_[0] == 0 && 
        chunks_[1] == 0 && 
        chunks_[2] == 0 && 
        chunks_[3] == 0;
    }

    constexpr uint64_t* chunks() noexcept
    {
        return chunks_;
    }

    constexpr const uint64_t* chunks() const noexcept
    {
        return chunks_;
    }

    void print(bool debug = true) const;

    private:
    uint64_t chunks_[CHUNK_NB];
};

inline Bitboard Bitboard::operator<<(int s) const noexcept
{
    Bitboard result;
    result.chunks_[0] = chunks_[0] << s;
    result.chunks_[1] = chunks_[1] << s;
    result.chunks_[2] = chunks_[2] << s;
    result.chunks_[3] = chunks_[3] << s;
    return result;
}

inline Bitboard Bitboard::operator>>(int s) const noexcept
{
    Bitboard result;
    result.chunks_[0] = chunks_[0] >> s;
    result.chunks_[1] = chunks_[1] >> s;
    result.chunks_[2] = chunks_[2] >> s;
    result.chunks_[3] = chunks_[3] >> s;
    return result;
}

} // namespace athena