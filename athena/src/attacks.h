#pragma once

#include "bitboard.h"
#include "chess.h"

namespace athena
{

    
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



// vertical
// horizontal

// Bishop PEXT tables
extern const std::array<std::array<uint64_t, 1024>, SQUARE_NB> PEXT_TABLE_DIAG;
extern const std::array<std::array<uint64_t, 1024>, SQUARE_NB> PEXT_TABLE_ANTI;

inline Bitboard genBishopAttacks(Square sq, const Bitboard& occupied) noexcept
{
    auto [diag, anti] = DIAGONAL[static_cast<uint8_t>(sq)];

    Bitboard bb1(occupied & diag);
    Bitboard bb2(occupied & anti);

    uint64_t mask1 = PEXT_TABLE_DIAG[static_cast<uint8_t>(sq)][_pext_u64(bb1.combine(), diag.combine())];
    uint64_t mask2 = PEXT_TABLE_ANTI[static_cast<uint8_t>(sq)][_pext_u64(bb2.combine(), anti.combine())];

    return (diag & mask1) | (anti & mask2);
}

// Rook PEXT tables
extern const std::array<std::array<Bitboard, 4096>, RANK_NB   > PEXT_TABLE_VERTICAL;
extern const std::array<std::array<uint64_t, 4096>, CHUNK_SIZE> PEXT_TABLE_HORIZONTAL;

inline Bitboard genRookAttacks(Square sq, const Bitboard& occupied) noexcept
{
    Bitboard attacks(0,0,0,0);

    const auto f = file(sq);
    const auto r = rank(sq);

    __m256i v = _mm256_loadu_si256((const __m256i*)occupied.chunks());
    __m256i s = _mm256_slli_epi64(v,  15 - f);

    // attacks |= PEXT_TABLE_VERTICAL[r][_pext_u32(_mm256_movemask_epi8(s), 0xAAAAAAAAU)];

    Bitboard vertical = PEXT_TABLE_VERTICAL[r][_pext_u32(_mm256_movemask_epi8(s), 0xAAAAAAAAU)];
    
    // Shift left by f amount without cross-chunk calculation
    __m256i vertical_v = _mm256_loadu_si256((const __m256i*)vertical.chunks());
    __m256i shifted = _mm256_slli_epi64(vertical_v, f);
    
    // Extract chunks and construct Bitboard
    alignas(32) uint64_t temp[4];
    _mm256_storeu_si256((__m256i*)temp, shifted);
    attacks |= Bitboard(temp[0], temp[1], temp[2], temp[3]);

    // const auto chk = chunk(sq);
    // const auto idx = index(sq);

    // attacks.chunks()[chk] |= PEXT_TABLE_HORIZONTAL[idx][_pext_u64(occupied.chunks()[chk], STRAIGHT[static_cast<uint8_t>(sq)].horizontal.chunks()[chk])];

    return attacks;
}

} // namespace athena