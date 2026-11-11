#pragma once

#include "chess/square.h"

namespace athena::nnue {

class Feature {
public:
    Feature(chess::Square sq, chess::Piece piece, chess::Color color, chess::Color perspective) noexcept {
        id_ = sq.rotate(perspective).id() +
        piece.id() * 160 +
        color.rotate(perspective).id() * 160 * chess::PIECE_NB;
    }

    int id() const noexcept {
        return id_;
    }

private:
    int id_;
};

} // namespace athena
