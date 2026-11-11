#pragma once

#include <cstddef>
#include <cstdint>

namespace athena::nnue {

constexpr int SCALE = 400;
constexpr int QA = 256;   // activation / SCReLU scale
constexpr int QB = 64;    // output-weight scale

// const HIDDEN_SIZE: usize = 128;
// const SCALE: i32 = 400;
// const QA: i16 = 255;
// const QB: i16 = 64;

enum { L1, L2, NNUE_LAYERS };
enum { W, B, PARAMETERS };
enum { ROW, COL, DIMENSIONS }; // AXIS

constexpr int ACCUMULATOR_SIZE = 64;

static constexpr int ARCH[NNUE_LAYERS][DIMENSIONS] = {
    {160*6*4, 64},
    {64*4, 1},
};

// static constexpr int ARCH_DIMENSIONS[NNUE_LAYERS][PARAMETERS] = {
//     {160*6*4, 64},
//     {64, 1},
// };

// std::size_t A0 = 160*6*4; // INPUT DIM
// std::size_t A1 = 64; 
// std::size_t A2 = 1;

enum { A0, A1, A2, DIMENSION };

constexpr std::size_t DIM[DIMENSION] = {
    160*6*4,
    64,
    1,
};

struct alignas(64) Network {
    int16_t w1[ ARCH[L1][ROW] ][ ARCH[L1][COL] ];
    int16_t b1[ ARCH[L1][COL] ];
    int16_t w2[ ARCH[L2][ROW] ][ ARCH[L2][COL] ];
    int16_t b2[ ARCH[L2][COL] ];
};



    // // Network architecture version
    // static constexpr uint version = 1;

    // // Network architecture
    // enum { L1, L2, NNUE_LAYERS };
    // enum { W, B, PARAMETERS };
    // enum { ROW, COL, DIMENSIONS };

    // // Matrix shapes
    // static constexpr uint ARCH[NNUE_LAYERS][DIMENSIONS] = {
    //     {160*6*4, 64}, // Layer1
    //     {1      , 64}, // Layer2
    // };

    // static constexpr uint ARCH_DIMENSIONS[NNUE_LAYERS][PARAMETERS] = {
    //     {ARCH[L1][ROW] * ARCH[L1][COL], ARCH[L1][COL]},
    //     {ARCH[L2][ROW] * ARCH[L2][COL], ARCH[L2][ROW]},
    // };

    // struct alignas(64) Network {
    //     int8_t  w1[ ARCH_DIMENSIONS[L1][W] ];
    //     int16_t b1[ ARCH_DIMENSIONS[L1][B] ];
    //     int8_t  w2[ ARCH_DIMENSIONS[L2][W] ];
    //     int32_t b2[ ARCH_DIMENSIONS[L2][B] ];
    // };

} // namespace athena::nnue
