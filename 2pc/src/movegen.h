#ifndef MOVEGEN_H
#define MOVEGEN_H

#include "position.h"
#include "move.h"

namespace Talia
{

[[nodiscard]] int genAllNoisyMoves(Position& pos, Move* moves);
[[nodiscard]] int genAllQuietMoves(Position& pos, Move* moves);
[[nodiscard]] int genAllLegalMoves(Position& pos, Move* moves);

} // namespace

#endif