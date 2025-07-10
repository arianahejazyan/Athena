#ifndef NNUE_NETWORK_H
#define NNUE_NETWORK_H

#include <cstdint>

namespace athena
{

    template <std::size_t L1, std::size_t L2>
    class Network
    {
    private:
        ReLU<L1> relu1;
        Dense<L1, L2> dense2;

        alignas(CacheLineSize) a1[L1];
        alignas(CacheLineSize) z2[L2];

    public:
        int32_t propagate(const int32_t *input)
        {
            relu1.propagate(input, a1);
            dense2.propagate(a1, z2);

            return z2[0];
        }
    };

} // namespace athena

#endif /* NNUE_NETWORK_H */