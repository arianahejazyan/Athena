#ifndef NNUE_CONFIG_H
#define NNUE_CONFIG_H

#include <cstdint>

namespace athena
{

    constexpr std::size_t L0 = 6 * 4 * 160; // 3840 input features
    constexpr std::size_t L1 = 128;         // Hidden layer size, example value
    constexpr std::size_t L2 = 1;

} // namespace athena

#endif /* NNUE_CONFIG_H */