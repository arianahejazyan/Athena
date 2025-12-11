// #ifndef NNUE_ACCUMULATOR_H
// #define NNUE_ACCUMULATOR_H

// #include <cstdint>
// #include "chess.h"

// namespace athena
// {

//     template <std::size_t size>
//     class Accumulator
//     {
//     private:
//         alignas(CacheLineSize) int data[size];
//     };

//     template <std::size_t size>
//     class AccumulatorStack
//     {
//     private:
//         std::vector<Accumulator<size>> stack;
//     };

// } // namespace athena

// #endif /* NNUE_ACCUMULATOR_H */