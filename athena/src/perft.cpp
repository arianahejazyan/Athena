#include "perft.h"
#include "movegen.h"
#include <vector>
#include <chrono>
#include <iostream>
#include <iomanip>
#include "utility.h"

namespace athena
{

std::string formatInt(uint64_t num)
{
    std::string numStr = std::to_string(num);
    int insertPosition = numStr.length() - 3;
    while (insertPosition > 0) {
        numStr.insert(insertPosition, ",");
        insertPosition -= 3;
    }
    return numStr;
}

void perft(Position& pos, Record& rc, int depth, bool full = false) noexcept
{
    const GameState& gs = pos.states.back();

    if (depth == 0)
    {
        rc.nodes++;
        return;
    }

    Move moves[MAX_MOVES];
    int size = 0; 
    size += genAllNoisyMoves(pos, moves + size);
    size += genAllQuietMoves(pos, moves + size);

    bool noLegalMove = true;
    for (int i = 0; i < size; ++i)
    {
        pos.makemove(moves[i]);

        if (isRoyalSafe(pos, gs.turn))
        {
            noLegalMove = false;

            if (depth == 1)
            {
                rc.nodes++;           
                
                if (full)
                {
                    auto flag = moves[i].flag();
                    switch (flag)
                    {
                        case Noisy: rc.noisy++; break;
                        case Quiet: rc.quiet++; break;
                    }

                    auto nature = moves[i].nature();
                    switch (nature)
                    {
                        case Jumper: rc.jumper++; break;
                        case Slider: rc.slider++; break;
                        case Pushed: rc.pushed++; break;
                        case Stride: rc.stride++; break;
                        case Strike: rc.strike++; break;
                        case Evolve: rc.evolve++; break;
                        case Enpass: rc.enpass++; break;
                        case Castle: rc.castle++; break;
                    }
                }   
            }

            else perft(pos, rc, depth - 1, full);
        }

        pos.undomove(moves[i]);
    }

    if (full && noLegalMove)
    {
        rc.nodes += 1;
        if (isRoyalSafe(pos, gs.turn)) rc.stalemates += 1;
        else                           rc.checkmates += 1;
    }
}

void runPerftTests(Position& pos, int depth, bool full, bool split, bool cumulative)
{
    std::cout << std::endl;

    if (split)
    {
        Move moves[MAX_MOVES];
        int size = 0;
        size += genAllNoisyMoves(pos, moves + size);
        size += genAllQuietMoves(pos, moves + size);

        uint64_t totalNodes = 0;
        double totalTime = 0.0;

        for (int i = 0; i < size; ++i)
        {
            pos.makemove(moves[i]);

            const GameState& gs = pos.states.back();

            if (!isRoyalSafe(pos, gs.turn))
            {
                pos.undomove(moves[i]);
                continue;
            }

            Record rc;

            auto start = std::chrono::high_resolution_clock::now();
            perft(pos, rc, depth - 1, false);
            auto end = std::chrono::high_resolution_clock::now();

            std::chrono::duration<double> elapsed = end - start;
            double seconds = elapsed.count();

            std::cout << toString(moves[i]) << ": " << rc.nodes << std::endl;

            totalNodes += rc.nodes;
            totalTime += seconds;

            pos.undomove(moves[i]);
        }

        std::cout << "total: " << formatInt(totalNodes) << " nodes, "
                  << std::fixed << std::setprecision(6) << totalTime << " s, "
                  << formatInt(static_cast<uint64_t>(totalNodes / totalTime)) << " nps\n";
    }
    else
    {
        if (full)
        {
            std::cout << std::setw(6) << "depth"
                      << std::setw(12) << "nodes"
                      << std::setw(10) << "quiet"
                      << std::setw(10) << "noisy"
                      << std::setw(10) << "jumper"
                      << std::setw(10) << "slider"
                      << std::setw(10) << "pushed"
                      << std::setw(10) << "stride"
                      << std::setw(10) << "strike"
                      << std::setw(10) << "evolve"
                      << std::setw(10) << "enpass"
                      << std::setw(10) << "castle"
                      << std::setw(12) << "stalemates"
                      << std::setw(13) << "checkmates"
                      << std::setw(12) << "time(s)"
                      << std::setw(15) << "nps" << "\n";
        }
        else
        {
            std::cout << std::setw(10) << "depth"
                      << std::setw(15) << "nodes"
                      << std::setw(15) << "time (s)"
                      << std::setw(15) << "nps" << "\n";
        }

        std::cout << std::string(full ? 185 : 55, '-') << "\n";

        Record cumulative_rc;
        Record total_rc;
        double totalTime = 0.0;

        for (int d = 1; d <= depth; ++d)
        {
            Record rc;

            auto start = std::chrono::high_resolution_clock::now();
            perft(pos, rc, d, full);
            auto end = std::chrono::high_resolution_clock::now();

            std::chrono::duration<double> elapsed = end - start;
            double seconds = elapsed.count();

            if (cumulative)
                cumulative_rc += rc;
            else
                cumulative_rc = rc;

            total_rc += cumulative_rc;
            totalTime += seconds;

            uint64_t nodes = cumulative ? cumulative_rc.nodes : rc.nodes;
            uint64_t nps = static_cast<uint64_t>(nodes / totalTime);

            if (full)
            {
                std::cout << std::setw(6) << d
                          << std::setw(12) << formatInt(nodes)
                          << std::setw(10) << formatInt(cumulative_rc.quiet)
                          << std::setw(10) << formatInt(cumulative_rc.noisy)
                          << std::setw(10) << formatInt(cumulative_rc.jumper)
                          << std::setw(10) << formatInt(cumulative_rc.slider)
                          << std::setw(10) << formatInt(cumulative_rc.pushed)
                          << std::setw(10) << formatInt(cumulative_rc.stride)
                          << std::setw(10) << formatInt(cumulative_rc.strike)
                          << std::setw(10) << formatInt(cumulative_rc.evolve)
                          << std::setw(10) << formatInt(cumulative_rc.enpass)
                          << std::setw(10) << formatInt(cumulative_rc.castle)
                          << std::setw(12) << formatInt(cumulative_rc.stalemates)
                          << std::setw(13) << formatInt(cumulative_rc.checkmates)
                          << std::setw(12) << std::fixed << std::setprecision(6) << totalTime
                          << std::setw(15) << formatInt(nps) << "\n";
            }
            else
            {
                std::cout << std::setw(10) << d
                          << std::setw(15) << formatInt(nodes)
                          << std::setw(15) << std::fixed << std::setprecision(6) << totalTime
                          << std::setw(15) << formatInt(nps) << "\n";
            }
        }

        std::cout << std::string(full ? 185 : 55, '-') << "\n";

        if (full)
        {
            std::cout << std::setw(6) << "total"
                      << std::setw(12) << formatInt(total_rc.nodes)
                      << std::setw(10) << formatInt(total_rc.quiet)
                      << std::setw(10) << formatInt(total_rc.noisy)
                      << std::setw(10) << formatInt(total_rc.jumper)
                      << std::setw(10) << formatInt(total_rc.slider)
                      << std::setw(10) << formatInt(total_rc.pushed)
                      << std::setw(10) << formatInt(total_rc.stride)
                      << std::setw(10) << formatInt(total_rc.strike)
                      << std::setw(10) << formatInt(total_rc.evolve)
                      << std::setw(10) << formatInt(total_rc.enpass)
                      << std::setw(10) << formatInt(total_rc.castle)
                      << std::setw(12) << formatInt(total_rc.stalemates)
                      << std::setw(13) << formatInt(total_rc.checkmates)
                      << std::setw(12) << std::fixed << std::setprecision(6) << totalTime
                      << std::setw(15) << formatInt(static_cast<uint64_t>(total_rc.nodes / totalTime)) << "\n";
        }
        else
        {
            std::cout << std::setw(10) << "total"
                      << std::setw(15) << formatInt(total_rc.nodes)
                      << std::setw(15) << std::fixed << std::setprecision(6) << totalTime
                      << std::setw(15) << formatInt(static_cast<uint64_t>(total_rc.nodes / totalTime)) << "\n";
        }
    }

    std::cout << std::endl;
}

} // namespace athena