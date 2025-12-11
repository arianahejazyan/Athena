#pragma once

#include "chess.h"

namespace athena
{

class alignas(CACHELINE_SIZE) Bitboard
{
    public:
    Bitboard() = default;

    constexpr Bitboard(const Bitboard& other) noexcept
        : chunks_{other.chunks_[0], other.chunks_[1], other.chunks_[2], other.chunks_[3]} {}

    constexpr Bitboard(uint64_t chunk0, uint64_t chunk1, uint64_t chunk2, uint64_t chunk3) noexcept
        : chunks_{chunk0, chunk1, chunk2, chunk3} {}

    Bitboard operator~() const noexcept
    {
        return Bitboard(
            ~chunks_[0], 
            ~chunks_[1], 
            ~chunks_[2], 
            ~chunks_[3]);
    }

    Bitboard& operator&=(const Bitboard& other) noexcept
    {
        for (std::size_t i = 0; i < CHUNK_NB; i++) {
            chunks_[i] &= other.chunks_[i];
        }
        return *this;
    }

    Bitboard& operator^=(const Bitboard& other) noexcept
    {
        for (std::size_t i = 0; i < CHUNK_NB; i++) {
            chunks_[i] ^= other.chunks_[i];
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

    constexpr void setSquare(Square sq) noexcept {
        chunks_[chunk(sq)] |= 1ULL << index(sq);
    }

    constexpr void popSquare(Square sq) noexcept {
        chunks_[chunk(sq)] &= ~(1ULL << index(sq));
    }

    constexpr bool hasSquare(Square sq) const {
        return (chunks_[chunk(sq)] >> index(sq)) & 0x1;
    }

    void print(bool debug = true) const;

    private:
    uint64_t chunks_[CHUNK_NB];
};

struct alignas(CACHELINE_SIZE) Straight
{
    Bitboard vertical;
    Bitboard horizontal;
};

struct alignas(CACHELINE_SIZE) Diagonal
{
    Bitboard diag;
    Bitboard anti;
};

struct alignas(CACHELINE_SIZE) Adjacent
{
    Bitboard knight;
    Bitboard king;
};

extern const std::array<Straight, SQUARE_NB> STRAIGHT;
extern const std::array<Diagonal, SQUARE_NB> DIAGONAL;
extern const std::array<Adjacent, SQUARE_NB> ADJACENT;

} // namespace athena