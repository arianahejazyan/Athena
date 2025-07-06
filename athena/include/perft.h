#pragma once

#include "position.h"

namespace athena
{

class Record
{
    public: uint64_t nodes = 0, 

    noisy = 0,
    quiet = 0,

    jumper = 0, slider = 0,
    pushed = 0, stride = 0,
    strike = 0, evolve = 0,
    enpass = 0, castle = 0,

    stalemates = 0,
    checkmates = 0;

    Record& operator+=(const Record& other)
    {
        nodes += other.nodes;
        noisy += other.noisy;
        quiet += other.quiet;

        jumper += other.jumper, slider += other.slider,
        pushed += other.pushed, stride += other.stride,
        strike += other.strike, evolve += other.evolve,
        enpass += other.enpass, castle += other.castle,

        stalemates += other.stalemates,
        checkmates += other.checkmates;

        return *this;
    }
};
  
void runPerftTests(Position& pos, int depth, bool full, bool split, bool cumulative);

} // namespace athena