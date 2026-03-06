#pragma once

#include "chess.h"
#include "constants.h"

namespace athena
{

std::size_t generate_noisy_moves(const Position& pos, Move* moves, const GameSetup setup) noexcept;
std::size_t generate_quiet_moves(const Position& pos, Move* moves, const GameSetup setup) noexcept;

} // namespace athena