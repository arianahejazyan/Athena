// #include <gtest/gtest.h>
// #include "nnue/layers/nnue_dense.h"

// using namespace athena;

// TEST(TestDense, Propagate)
// {
//     constexpr std::size_t inSize = 3;
//     constexpr std::size_t outSize = 2;

//     Dense<inSize, outSize> dense;

//     // Set weights manually
//     auto &weights = dense.weights();
//     weights[0][0] = 1;
//     weights[0][1] = 2;
//     weights[0][2] = 3;
//     weights[1][0] = 4;
//     weights[1][1] = 5;
//     weights[1][2] = 6;

//     // Set biases
//     auto &biases = dense.biases();
//     biases[0] = 10;
//     biases[1] = 20;

//     // Input vector
//     int32_t input[inSize] = {1, 2, 3};

//     // Output buffer
//     int32_t output[outSize] = {};

//     // Call propagate
//     dense.propagate(input, output);

//     // Expected calculation:
//     // output[0] = 10 + 1*1 + 2*2 + 3*3 = 24
//     // output[1] = 20 + 4*1 + 5*2 + 6*3 = 52

//     EXPECT_EQ(output[0], 24);
//     EXPECT_EQ(output[1], 52);
// }
