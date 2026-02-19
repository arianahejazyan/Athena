#pragma once

#include "chess.h"

namespace athena
{

std::size_t generate_noisy_moves(const Position& pos, Move* moves);
std::size_t generate_quiet_moves(const Position& pos, Move* moves);

} // namespace athena