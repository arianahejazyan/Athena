#include "eval.h"
#include <cstdint>

namespace athena {

Score evaluate(const chess::Position& pos) noexcept {
    const auto turn = pos.turn();
    Score score = 0;
    for (int sq = 20; sq <= 235; sq++) {
        const auto pc = pos.board(static_cast<chess::Square::ID>(sq));
        const auto piece = pc.piece().id();
        const auto color = pc.color().id();

        int sign = 1 - 2 * turn.diff(color);
        score += sign * material[static_cast<uint8_t>(piece)];
    }
    return score;
}

} // namespace athena