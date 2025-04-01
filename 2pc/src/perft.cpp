#include "perft.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include "movegen.h"
#include "uci.h"
#include "utility.h"

namespace Talia
{

[[nodiscard]] uint64_t perft(Position& pos, int depth)
{
    if (depth == 0) return 1;

    StaticArray<Move, MAX_MOVES> moves;
    const auto size = genAllLegalMoves(pos, moves.begin());
    uint64_t nodes = 0;

    // bulk optimaztion
    if (depth == 1) return size;
    
    for (int i = 0; i < size; ++i)
    {
        pos.makemove(moves[i]);
        nodes += perft(pos, depth - 1);
        pos.undomove(moves[i]);
    }

    return nodes;
}

[[nodiscard]] std::vector<std::pair<Move, uint64_t>> splitPerft(Position& pos, int depth)
{
    std::vector<std::pair<Move, uint64_t>> results;

    StaticArray<Move, MAX_MOVES> moves;
    const int size = genAllLegalMoves(pos, moves.begin());

    for (int i = 0; i < size; ++i)
    {
        pos.makemove(moves[i]);
        uint64_t count = perft(pos, depth - 1);
        pos.undomove(moves[i]);

        results.emplace_back(moves[i], count);
    }

    return results;
}

void printPerftResult(const Position& pos, const int depth, const bool split)
{
    if (split)
    {
        Position testPos = pos;

        uint64_t total = 0;
        const auto results = splitPerft(testPos, depth);
        for (const auto result: results)
        {
            std::cout << result.first.toUCI() << ": " << result.second << std::endl;
            total += result.second;
        }

        std::cout << "Total: " << total << std::endl;
    }

    else
    {
        std::cout << std::setw(10) << "Depth"    << std::setw(15) << "Nodes" 
             << std::setw(15) << "Time (s)" << std::setw(15) << "NPS" << std::endl;
        std::cout << string(60, '-') << std::endl;          
        
        u64 totalNodes = 0;
        double totalTime = 0.0;
        u64 result;

        for (int d = 1; d <= depth; ++d)
        {
            Position testPos = pos;
            
            const auto start = std::chrono::high_resolution_clock::now();
            result = perft(testPos, d);
            const auto end = std::chrono::high_resolution_clock::now();
            
            std::chrono::duration<double> elapsed = end - start;
            double seconds = elapsed.count();
            uint64_t nps = static_cast<uint64_t>(result / seconds);
            
            totalNodes += result;
            totalTime += seconds;
            
            std::cout << std::setw(10) << depth 
                 << std::setw(15) << formatNumber(result)
                 << std::setw(15) << std::fixed << std::setprecision(6) << seconds
                 << std::setw(15) << formatNumber(nps) << std::endl;            
        }

        std::cout << string(60, '-') << std::endl;
        std::cout << std::setw(10) << "Total"
             << std::setw(15) << formatNumber(totalNodes)
             << std::setw(15) << std::fixed << std::setprecision(6) << totalTime
             << std::setw(15) << formatNumber(static_cast<u64>(totalNodes / totalTime)) << std::endl;
    }
}

} // namespace