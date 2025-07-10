#ifndef DENSE_H
#define DENSE_H

#include <cstdint>

namespace athena
{

    template <std::size_t inSize, std::size_t outSize>
    class Dense
    {
    private:
        alignas(CacheLineSize) int32_t weights[outSize][inSize];
        alignas(CacheLineSize) int32_t biases[outSize];

    public:
        void propagate(const int32_t *input, int32_t *output)
        {
            for (int j = 0; j < outSize; ++j)
            {
                output[j] = biases[j];
            }

            for (int j = 0; j < inSize; ++j)
            {
                for (int i = 0; i < outSize; ++i)
                {
                    output[i] += weights[i][j] * input[j];
                }
            }
        }
    };

} // namespace athena

#endif /* DENSE_H */