#pragma once

#include <cstdint>
#include "chess/position.h"
#include "chess/movegen.h"

namespace athena::chess {

inline uint64_t perft(chess::Position& pos, int depth) {

    if (depth == 0) return 1;

    chess::Move moves[chess::MOVE_NB];
    int num_moves = generate_legal_moves(pos, moves);

    if (depth == 1)
        return static_cast<uint64_t>(num_moves);

    uint64_t nodes = 0;
    for (int i = 0; i < num_moves; i++) {
        const auto& move = moves[i];
        pos.make_move(move);
        nodes += perft(pos, depth - 1);
        pos.undo_move(move);
    }

    return nodes;
}

} // namespace athena