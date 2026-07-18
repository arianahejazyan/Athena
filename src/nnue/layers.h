#pragma once

#include <algorithm>
#include <cstddef>
#include "nnue/simd.h"

namespace athena::nnue {

template<std::size_t SIZE>
void clipped_relu(float* __restrict__ input, float min_value, float max_value) noexcept {
    for (std::size_t i = 0; i < SIZE; ++i) {
        input[i] = std::min(max_value, std::max(min_value, input[i]));
    }
}

template<std::size_t SIZE>
void add(float* __restrict__ accumulator, const float* __restrict__ weights) noexcept {
    for (std::size_t i = 0; i < SIZE; ++i) {
        accumulator[i] += weights[i];
    }
}

template<std::size_t SIZE>
void sub(float* __restrict__ accumulator, const float* __restrict__ weights) noexcept {
    for (std::size_t i = 0; i < SIZE; ++i) {
        accumulator[i] -= weights[i];
    }
}

template<std::size_t INPUT_SIZE, std::size_t OUTPUT_SIZE>
void affine(const float* __restrict__ input, float* __restrict__ output, const float* __restrict__ weights, const float* __restrict__ biases) noexcept {

    // Compute matrix multiplication: output = weights * input
    for (std::size_t o = 0; o < OUTPUT_SIZE; ++o) {
        const float* __restrict__ w = &weights[o * INPUT_SIZE];
        float sum = 0.0f;

        for (std::size_t i = 0; i < INPUT_SIZE; ++i) {
            sum += w[i] * input[i];
        }

        output[o] = sum;
    }
    
    // Then add bias vector
    add<OUTPUT_SIZE>(output, biases);
}

} // namespace athena