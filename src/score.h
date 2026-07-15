#pragma once

#include "chess/constants.h"
#include <cstdint>

namespace athena {

using Score = int32_t;

constexpr Score SCORE_INFINITE = 999999;
constexpr Score SCORE_MATE     = 100000;
constexpr Score SCORE_DRAW     = 0;

inline Score abs(Score score) noexcept {
    return score >= 0 ? score : -score;
}

inline bool isMateScore(Score score) noexcept {
    return SCORE_MATE - chess::PLAY_NB <= abs(score);
}

inline int mateIn(Score score) noexcept {
    return SCORE_MATE - abs(score);
}

} // namespace athena