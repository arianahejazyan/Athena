#ifndef RELU_H
#define RELU_H

#include <algorithm>

namespace athena
{

    template <std::size_t Size>
    class ReLU
    {
    public:
        void propagatge(const int *input, int *output) const
        {
            for (int i = 0; i < Size; ++i)
            {
                output[i] = std::max(0, input[i]);
            }
        }
    };

} // namespace athena

#endif /* RELU_H */