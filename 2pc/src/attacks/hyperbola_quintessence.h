#ifndef HYPERBOLA_QUINTESSENCE_H
#define HYPERBOLA_QUINTESSENCE_H

#include <cassert>
#include "bitboard.h"
#include "chess.h"
#include "types.h"

namespace Talia
{

extern const StaticArray<BitBoard, SQUARE_NB> DIAGONAL_MASK;
extern const StaticArray<BitBoard, SQUARE_NB> ANTIDIAGONAL_MASK;
extern const StaticArray<BitBoard, SQUARE_NB> VERTICAL_MASK;
extern const StaticArray<BitBoard, FILE_NB * 64> HORIZONTAL_MASK;

template<Piece piece>
inline auto hyperbolaQuintessence(const Square sq, const BitBoard occ)
{
    assert((piece == QUEEN) || (piece == ROOK) || (piece == BISHOP));

    BitBoard attack = EMPTY_BOARD;

    if constexpr (piece == QUEEN || piece == BISHOP)
    {
        const BitBoard m = DIAGONAL_MASK[sq];
        const BitBoard o = m & occ;

        setbb(attack, ((o - BB(sq)) ^ swap(swap(o) - (0x8000000000000000ull >> sq))) & m);
    }

    if constexpr (piece == QUEEN || piece == BISHOP)
    {
        const BitBoard m = ANTIDIAGONAL_MASK[sq];
        const BitBoard o = m & occ;

        setbb(attack, ((o - BB(sq)) ^ swap(swap(o) - (0x8000000000000000ull >> sq))) & m);
    }

    if constexpr (piece == QUEEN || piece == ROOK)
    {
        const BitBoard m = VERTICAL_MASK[sq];        
        const BitBoard o = m & occ;

        setbb(attack, ((o - BB(sq)) ^ swap(swap(o) - (0x8000000000000000ull >> sq))) & m);
    }

    if constexpr (piece == QUEEN || piece == ROOK)
    {
        const auto f = sq & 7;
        const auto r = sq & 56;
        const BitBoard o = (occ >> r) & 126;

        setbb(attack, (HORIZONTAL_MASK[(o << 2) + f]) << r);
    }

    return attack;
}

} // namespace

#endif