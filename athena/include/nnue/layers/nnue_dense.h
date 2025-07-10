#ifndef NNUE_DENSE_H
#define NNUE_DENSE_H

#include <cstdint>
#include "../nnue_config.h"

namespace athena
{

    template <std::size_t inSize, std::size_t outSize>
    class Dense
    {
    private:
        alignas(CacheLineSize) int32_t weights_[outSize][inSize];
        alignas(CacheLineSize) int32_t biases_[outSize];

    public:
        int32_t (&weights())[outSize][inSize] { return weights_; }
        int32_t (&biases())[outSize] { return biases_; }

        const int32_t (&weights() const)[outSize][inSize] { return weights_; }
        const int32_t (&biases() const)[outSize] { return biases_; }

        void propagate(const int32_t *input, int32_t *output)
        {
            for (int j = 0; j < outSize; ++j)
            {
                output[j] = biases_[j];
            }

            for (int j = 0; j < inSize; ++j)
            {
                for (int i = 0; i < outSize; ++i)
                {
                    output[i] += weights_[i][j] * input[j];
                }
            }
        }
    };

} // namespace athena

#endif /* NNUE_DENSE_H */