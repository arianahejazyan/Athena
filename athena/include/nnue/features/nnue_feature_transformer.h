#ifndef NNUE_FEATURE_TRANSFORMER_H
#define NNUE_FEATURE_TRANSFORMER_H

#include <cstring>
#include "nnue_feature.h"
#include "position.h"
#include "../nnue_config.h"

namespace athena
{

    class FeatureTransformer
    {
    public:
        void transform(const Position &pos, int *feature_vector) const
        {
            std::memset(feature_vector, 0, sizeof(int) * Lx0);
            for (auto sq : VALID_SQUARES)
            {
                auto pc = pos.board[sq];
                if (pc != EMPTY)
                {
                    auto idx = feature_index(sq, pc);
                    feature_vector[idx] = 1;
                }
            }
        }
    };

} // namespace athena

#endif /* NNUE_FEATURE_TRANSFORMER_H */