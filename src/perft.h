#pragma once

#include <vector>
#include "chess.h"
#include "constants.h"

namespace athena
{

class Perft
{
    public: 
    struct Record
    {
        uint64_t nodes = 0;
        // uint64_t delta = 0;
    };

    struct Options // make enum for mode or something
    {
        int depth;
        bool split;
        bool tabular;
    };

    void run(const Position& pos, const Options& options, const GameSetup setup);
    void print() const;

    Options options_;
    std::vector<Record> records_;

    uint64_t perft(Position& pos, int depth, const GameSetup setup);
    void printSimple() const;
    void printSplit() const;
    void printTabular() const;
};

} // namespace athena