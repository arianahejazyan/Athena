#pragma once

#include "chess/square.h"

namespace athena::nnue {

inline uint make_index(chess::Square sq, chess::Piece piece, chess::Color color) noexcept {
    return 
        static_cast<uint8_t>(sq.compress().id()) +
        static_cast<uint8_t>(piece.id()) * chess::SQUARE_NB +
        static_cast<uint8_t>(color.id()) * chess::SQUARE_NB * chess::PIECE_NB;
}

} // namespace athena