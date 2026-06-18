#pragma once

#include "move.h"
#include "position.h"

namespace athena::chess {

[[nodiscard]] int generate_noisy_moves(Position& pos, Move* moves) noexcept;
[[nodiscard]] int generate_quiet_moves(Position& pos, Move* moves) noexcept;
[[nodiscard]] int generate_legal_moves(Position& pos, Move* moves) noexcept;

} // namespace athena