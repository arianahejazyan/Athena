#pragma once

#include "chess.h"

namespace athena
{

std::size_t genAllPseudoMoves(const Position& pos, Move* moves);

} // namespace athena