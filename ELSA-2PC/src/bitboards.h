#ifndef BITBOARD_H
#define BITBOARD_H

#include "chess.h"

namespace ELSA
{
    
Square makeSquare(u32 rank, u32 file);

u64 BB(Square sq);

u32 getRank(u32 sq);

u32 getFile(u32 sq);

}; // namespace

#endif