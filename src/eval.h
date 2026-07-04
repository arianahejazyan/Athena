#pragma once

#include "chess/constants.h"
#include "chess/position.h"
#include "score.h"
#include <array>

namespace athena {

inline std::array<Score, chess::PIECE_NB> material = {
    0,    // King
    300,  // Knight
    350,  // Bishop
    500,  // Rook
    1000, // Queen
    100,  // Pawn
};

Score evaluate(const chess::Position& pos) noexcept;

} // namespace athena