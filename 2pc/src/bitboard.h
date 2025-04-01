#ifndef BITBOARD_H
#define BITBOARD_H

#include <iostream>
#include <cassert>
#include "chess.h"
#include "types.h"

namespace Talia
{

constexpr inline bool isValidSquare(int r, int f)
{
    bool cond1 = 0 <= r && r <= 7;
    bool cond2 = 0 <= f && f <= 7;

    return cond1 && cond2;
}

constexpr inline Square makeSquare(int r, int f)
{
    assert(0 <= r && r <= 7);
    assert(0 <= f && f <= 7);

    return (Square)(r * FILE_NB + f); 
}

constexpr inline BitBoard BB(Square sq)
{
    assert(A1 <= sq && sq <= H8);

    return 1ull << sq;
}

constexpr inline int rankSQ(Square sq) 
{
    assert(A1 <= sq && sq <= H8);

    return (sq >> 0x3); 
}

constexpr inline int fileSQ(Square sq) 
{ 
    assert(A1 <= sq && sq <= H8);

    return (sq & 0x7); 
}

constexpr inline void setbb(BitBoard& bb, BitBoard bit)
{
    bb |= bit;
}

constexpr inline void setbb(BitBoard& bb, Square bit)
{
    setbb(bb, BB(bit));
}

constexpr inline void setbb(Castle& castle, BitBoard bit)
{
    castle = static_cast<Castle>(castle | bit);
}

constexpr inline void popbb(BitBoard& bb, Square bit)
{
    assert(A1 <= bit && bit <= H8);

    bb &= ~BB(bit);
}

constexpr inline void popbb(Castle& bb, Square bit)
{
    bb = static_cast<Castle>(bb & ~BB(bit));
}

constexpr inline Square getlsb(BitBoard bb)
{
    return (Square)__builtin_ctzll(bb);
}

constexpr inline Square getmsb(u64 bb)
{
    assert(bb);

    return (Square)(__builtin_clzll(bb) ^ 63);
}

constexpr inline Square poplsb(u64& bb)
{
    Square lsb = getlsb(bb);
    bb &= bb - 1;
    return lsb;
}

constexpr inline Square popmsb(u64& bb)
{
    Square msb = getmsb(bb);
    bb ^= 1ull << msb;
    return msb;
}

constexpr inline int countbb(u64 bb)
{
    return __builtin_popcountll(bb);
}

[[nodiscard]] constexpr inline Square flipSQ(const Square sq)
{
    return static_cast<Square>(sq ^ 56);
}

[[nodiscard]] constexpr inline bool hasCastlingRight(const Castle castle, const Castle color, const Castle side)
{
    assert(color == WHITESIDE || color == BLACKSIDE);
    assert(side  == KINGSIDE  || side  == QUEENSIDE);

    const Castle mask = RIGHT & color & side;

    return (castle & mask) == mask;
}

constexpr inline u64 homerank(const Color turn)
{
    return turn == WHITE ? RANK3 : RANK6;
}

constexpr inline u64 promotionrank(const Color turn)
{
    return turn == WHITE ? RANK7 : RANK2;
}

constexpr inline u64 enpassrank(const Color turn)
{
    return turn == WHITE ? RANK5 : RANK4;
}

template<Offset offset>
static inline u64 shift(const u64 bb)
{
    switch (offset)
    {
        case NORTH: return bb << 8;
        case SOUTH: return bb >> 8;

        case EAST: return (bb & ~FILEH) << 1;
        case WEST: return (bb & ~FILEA) >> 1;

        case NE: return (bb & ~FILEH) << 9;
        case NW: return (bb & ~FILEA) << 7;
        case SE: return (bb & ~FILEH) >> 7;
        case SW: return (bb & ~FILEA) >> 9;

        default: return 0ull;
    }
}


constexpr inline u64 rankbb(Square sq)
{
    const auto r = rankSQ(sq);

    switch (r)
    {
        case 0: return RANK1;
        case 1: return RANK2;
        case 2: return RANK3;
        case 3: return RANK4;
        case 4: return RANK5;
        case 5: return RANK6;
        case 6: return RANK7;
        case 7: return RANK8;

        default: return 0ULL;
    }
}

[[nodiscard]] bool checkbb(BitBoard bb, Square sq);

void printbb(BitBoard bb);

inline BitBoard swap(BitBoard b)
{
#if defined(__GNUC__)
    return __builtin_bswap64(b);
#else
    b = ((b >> 8)  & 0x00FF00FF00FF00FFULL) | ((b << 8)  & 0xFF00FF00FF00FF00ULL);
    b = ((b >> 16) & 0x0000FFFF0000FFFFULL) | ((b << 16) & 0xFFFF0000FFFF0000ULL);
    b = ((b >> 32) & 0x00000000FFFFFFFFULL) | ((b << 32) & 0xFFFFFFFF00000000ULL);
    return b;
#endif
}

}; // namespace

#endif