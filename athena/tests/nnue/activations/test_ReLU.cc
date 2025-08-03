// #include <gtest/gtest.h>
// #include "nnue/activations/nnue_ReLU.h"

// TEST(TestReLU, Propagate)
// {
//     constexpr std::size_t size = 5;
//     const int input[] = {-3, 0, 5, -10, 7};
//     int output[size] = {};

//     athena::ReLU<size> ReLU;
//     ReLU.propagatge(input, output);

//     const int expected[] = {0, 0, 5, 0, 7};
//     for (int i = 0; i < size; ++i)
//     {
//         EXPECT_EQ(output[i], expected[i]);
//     }
// }