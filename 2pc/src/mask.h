#ifndef MAKSK_H
#define MAKSK_H

#include "bitboard.h"
#include "chess.h"
#include "types.h"

namespace Talia
{

extern const StaticArray<BitBoard, SQUARE_NB, PIECE_NB> PIECE_MASK;
extern const StaticArray<BitBoard, SQUARE_NB, COLOR_NB> COLOR_MASK;
extern const StaticArray<BitBoard, SQUARE_NB, SQUARE_NB> LINE;
extern const StaticArray<BitBoard, SQUARE_NB, SQUARE_NB> BETWEEN;

} // namespace

#endif