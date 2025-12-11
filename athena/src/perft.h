#pragma once

#include <vector>
#include "chess.h"

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

    struct Options
    {
        int depth;
        bool split;
        bool tabular;
    };

    void run(const Position& pos, const Options& options);
    void print() const;

    private:
    Options options_;
    std::vector<Record> records_;

    uint64_t perft(Position& pos, int depth);
    void printSimple() const;
    void printSplit() const;
    void printTabular() const;
};

} // namespace athena