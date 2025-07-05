#ifndef MOVEGEN_H
#define MOVEGEN_H

#include "chess.h"
#include "position.h"

namespace athena
{

//
bool isSquareAttacked(const Position& pos, Square source, Color color) noexcept;

//
bool isRoyalSafe(const Position& pos, Color color) noexcept;

// 
std::size_t genAllNoisyMoves(const Position& pos, Move* moves);
std::size_t genAllQuietMoves(const Position& pos, Move* moves);

} // namespace athena

#endif // #ifndef MOVEGEN_H