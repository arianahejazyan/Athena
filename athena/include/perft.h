#ifndef PERFT_H
#define PERFT_H

#include <vector>
#include "position.h"

namespace athena
{



class Record
{
    public: u64 nodes = 0, 

    noisy = 0,
    quiet = 0,

    jumper = 0, slider = 0,
    pushed = 0, stride = 0,
    strike = 0, evolve = 0,
    enpass = 0, castle = 0,

    // draw = 0,
    // win  = 0,
    // lose = 0,

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

        // draw += other.draw,
        // win  += other.win ,
        // lose += other.lose,

        stalemates += other.stalemates,
        checkmates += other.checkmates;

        return *this;
    }
};

class Perft
{
    public:
    std::vector<Record> records;

    Perft() {}

    void run(Position& pos, int depth, bool full, bool split, bool cumulative);

    void print() const;
};
  
// void runPerftTests(Position& pos, int depth, bool full, bool split, bool cumulative);

} // namespace athena

#endif /* PERFT_H */