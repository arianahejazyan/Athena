#ifndef ATTACKS_H
#define ATTACKS_H

#include "hyperbola_quintessence.h"
#include "magic.h"
#include "chess.h"
#include "mask.h"
#include "types.h"

namespace Talia
{

template<Piece piece>
inline auto attacks(const Square sq, const u64 occ)
{
    assert((piece == QUEEN) || (piece == ROOK) || (piece == BISHOP));
    
    return hyperbolaQuintessence<piece>(sq, occ);
}

inline auto attacks(const Square sq, const Piece piece)
{
    assert((piece == KING) || (piece == KNIGHT));

    return PIECE_MASK[sq][piece];
}

inline auto attacks(const Square sq, const Color color)
{
    assert((color == WHITE) || (color == BLACK));

    return COLOR_MASK[sq][color];
}

} // namespace

#endif