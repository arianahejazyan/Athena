#pragma once

#include "bitboard.h"
#include "chess.h"

namespace athena
{

inline uint64_t pext_fallback(uint64_t src, uint64_t mask) noexcept
{
    uint64_t result = 0;
    uint64_t bit = 1;
    while (mask)
    {
        const uint64_t lsb = mask & (~mask + 1);
        if (src & lsb) result |= bit;
        mask ^= lsb;
        bit <<= 1;
    }
    return result;
}

inline uint64_t lzcnt_fallback(uint64_t value) noexcept
{
    if (value == 0) return 64;
#if defined(_MSC_VER)
    unsigned long index = 0;
#if defined(_M_X64) || defined(_M_ARM64)
    _BitScanReverse64(&index, value);
#else
    if ((value >> 32) != 0)
    {
        _BitScanReverse(&index, static_cast<unsigned long>(value >> 32));
        index += 32;
    }
    else
    {
        _BitScanReverse(&index, static_cast<unsigned long>(value));
    }
#endif
    return 63 - index;
#else
    return static_cast<uint64_t>(__builtin_clzll(value));
#endif
}

    
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

inline Bitboard bishop_attacks(Square sq, const Bitboard& occupied) noexcept
{
#if (defined(__x86_64__) || defined(__i386__) || defined(_M_X64) || defined(_M_IX86)) && defined(__BMI2__)
    const auto [diag, anti] = DIAGONAL[static_cast<uint8_t>(sq)];
    const Bitboard bb1(occupied & diag);
    const Bitboard bb2(occupied & anti);

    const uint64_t mask1 = PEXT_TABLE_DIAG[static_cast<uint8_t>(sq)][_pext_u64(bb1.combine(), diag.combine())];
    const uint64_t mask2 = PEXT_TABLE_ANTI[static_cast<uint8_t>(sq)][_pext_u64(bb2.combine(), anti.combine())];

    return (diag & mask1) | (anti & mask2);
#else
    const auto [diag, anti] = DIAGONAL[static_cast<uint8_t>(sq)];
    const Bitboard bb1(occupied & diag);
    const Bitboard bb2(occupied & anti);

    const uint64_t mask1 = PEXT_TABLE_DIAG[static_cast<uint8_t>(sq)][pext_fallback(bb1.combine(), diag.combine())];
    const uint64_t mask2 = PEXT_TABLE_ANTI[static_cast<uint8_t>(sq)][pext_fallback(bb2.combine(), anti.combine())];

    return (diag & mask1) | (anti & mask2);
#endif
}

// Rook PEXT tables
extern const std::array<std::array<Bitboard, 4096>, RANK_NB   > PEXT_TABLE_VERTICAL;
extern const std::array<std::array<uint64_t, 4096>, CHUNK_SIZE> PEXT_TABLE_HORIZONTAL;

inline Bitboard rook_attacks(Square sq, const Bitboard& occupied) noexcept
{
#if (defined(__x86_64__) || defined(__i386__) || defined(_M_X64) || defined(_M_IX86)) && defined(__AVX2__) && defined(__BMI2__) && defined(__LZCNT__)
    Bitboard attacks(0,0,0,0);

    // Vertical attacks
    const auto f = sq.file();
    const auto r = sq.rank();

    const __m256i v = _mm256_loadu_si256((const __m256i*)occupied.chunks_);
    const __m256i s = _mm256_slli_epi64(v,  15 - f);

    Bitboard vertical = PEXT_TABLE_VERTICAL[r][_pext_u32(_mm256_movemask_epi8(s), 0xAAAAAAAAU)];

    // Shift left by f amount without cross-chunk calculation
    const __m256i vertical_v = _mm256_loadu_si256((const __m256i*)vertical.chunks_);
    const __m256i shifted = _mm256_slli_epi64(vertical_v, f);

    // Extract chunks and construct Bitboard
    alignas(32) uint64_t temp[4];
    _mm256_storeu_si256((__m256i*)temp, shifted);
    attacks |= Bitboard(temp[0], temp[1], temp[2], temp[3]);

    // Horizontal attacks
    const auto chk = sq.chunk();
    const auto shift = (r & 0b11) * FILE_NB;
    const auto occ = occupied.chunks_[chk] >> shift;

    const auto upper = occ & (0xFFFFULL << (1  + f));
    const auto lower = occ & (0xFFFFULL >> (16 - f));

    const auto ms1B = 0x8000000000000000ULL >> _lzcnt_u64(lower | 1);
    const auto diff = upper ^ (upper - ms1B);

    attacks.chunks_[chk] |= (diff & (0x7FFEULL ^ (1 << f))) << shift;

    return attacks;
#else
    Bitboard attacks(0,0,0,0);
    const auto f = sq.file();
    const auto r = sq.rank();

    // Vertical attacks using scalar file occupancy extraction.
    uint32_t vertical_occ = 0;
    for (int rank = 1; rank <= 14; ++rank)
    {
        if (occupied.has(Square(rank, f)))
        {
            vertical_occ |= (1U << rank);
        }
    }
    Bitboard vertical = PEXT_TABLE_VERTICAL[r][vertical_occ];
    vertical << f;
    attacks |= vertical;

    // Horizontal attacks using scalar bit tricks.
    const auto chk = sq.chunk();
    const auto shift = (r & 0b11) * FILE_NB;
    const auto occ = occupied.chunks_[chk] >> shift;

    const auto upper = occ & (0xFFFFULL << (1 + f));
    const auto lower = occ & (0xFFFFULL >> (16 - f));

    const auto ms1B = 0x8000000000000000ULL >> lzcnt_fallback(lower | 1);
    const auto diff = upper ^ (upper - ms1B);

    attacks.chunks_[chk] |= (diff & (0x7FFEULL ^ (1 << f))) << shift;

    return attacks;
#endif
}

inline Bitboard bishop_attacks(Square sq) noexcept
{
    return 
    DIAGONAL[static_cast<uint8_t>(sq)].diag | 
    DIAGONAL[static_cast<uint8_t>(sq)].anti;
}

inline Bitboard rook_attacks(Square sq) noexcept
{
    return 
    STRAIGHT[static_cast<uint8_t>(sq)].vertical | 
    STRAIGHT[static_cast<uint8_t>(sq)].horizontal;
}

// ===== between mask (template specialization) ===== //

template<uint8_t piece>
inline Bitboard between_mask(Square sq1, Square sq2) noexcept;

template<>
inline Bitboard between_mask<Piece::Bishop>(Square sq1, Square sq2) noexcept
{
    return 
    bishop_attacks(sq1) & 
    bishop_attacks(sq2) & subtract(sq1, sq2);
}

template<>
inline Bitboard between_mask<Piece::Rook>(Square sq1, Square sq2) noexcept
{
    return 
    rook_attacks(sq1) & 
    rook_attacks(sq2) & subtract(sq1, sq2);
}

} // namespace athena
