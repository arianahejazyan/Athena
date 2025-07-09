#pragma once

#include <cstring>
#include <cstdint>
#include "position.h"

namespace athena
{

    constexpr std::size_t L0 = 6 * 4 * 160; // 3840 input features
    constexpr std::size_t L1 = 128;         // Hidden layer size, example value
    constexpr std::size_t L2 = 1;

    int make_index(Square sq, PieceClass pc)
    {
        auto piece = pc.piece();
        auto color = pc.color();
        return color + (piece * 4) + (sq * 24);
    }

    class FeatureTransformer
    {
    public:
        void transform(const Position &pos, int *feature_vector) const
        {
            std::memset(feature_vector, 0, sizeof(int) * L0);
            for (auto sq : VALID_SQUARES)
            {
                auto pc = pos.board[sq];
                if (pc != EMPTY)
                {
                    auto idx = make_index(sq, pc);
                    feature_vector[idx] = 1;
                }
            }
        }
    };

} // namespace athena
