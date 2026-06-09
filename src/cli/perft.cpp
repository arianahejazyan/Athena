#include "perft.h"
#include "constants.h"
#include "movegen.h"
#include "position.h"
// #include <vector>
// #include <chrono>
// #include <iostream>
// #include <iomanip>
// #include "utility.h"
#include <chrono>
#include <cstdint>
#include <iostream>

namespace athena
{

std::string formatInt(uint64_t num)
{
    std::string numStr = std::to_string(num);
    int insertPosition = numStr.length() - 3;
    while (insertPosition > 0)
    {
        numStr.insert(insertPosition, ",");
        insertPosition -= 3;
    }
    return numStr;
}

void Perft::run(const Position& pos, const Options& options, const GameSetup setup)
{
    Position position = pos;
    options_ = options;
    // records_.clear();

    if (options_.split)
    {
        Position x = pos;

        uint64_t count = 0;

        // x.movegen();
        Move moves[MOVE_NB];
        std::size_t n = 0;
        n = generate_noisy_moves(x, moves + n, setup);
        n = generate_quiet_moves(x, moves + n, setup);

        for (std::size_t i = 0; i < n; ++i)
        {
            Position x = pos;
            x.makemove(moves[i], setup);
            
            if (x.inCheck(x.turn().prev()))
            {
                x.undomove(moves[i], setup);
                continue;
            }

            uint64_t nodes = perft(x, options_.depth - 1, setup);
            // records_[options_.depth].nodes += nodes;
            // records_[options_.depth].delta += nodes;
            count += nodes;

            std::cout << moves[i].uci(true) << ": " << nodes << std::endl;

            x.undomove(moves[i], setup);
        }
        std::cout << "total: " << count <<std::endl;
        return;
    }

    // else if (options_.tabular)
    // {

    // }

    // else
    // {
        
    // }

    std::chrono::high_resolution_clock::time_point tic = std::chrono::high_resolution_clock::now();
    uint64_t nodes = perft(position, options_.depth, setup);
    std::chrono::high_resolution_clock::time_point toc = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = toc - tic;
    double seconds = elapsed.count();

    // std::cout << "time: "  << seconds << " seconds" << std::endl;
    std::cout << "nodes: " << formatInt(nodes) << std::endl;
    std::cout << "nps: "   << formatInt(static_cast<uint64_t>(nodes / seconds)) << std::endl;
}

uint64_t Perft::perft(Position& pos, int depth, const GameSetup setup)
{
    if (depth == 0)
        return 1;

    Move moves[MOVE_NB];
    std::size_t n = 0;
    n = generate_noisy_moves(pos, moves + n, setup);
    n = generate_quiet_moves(pos, moves + n, setup);

    // if (depth == 1)
    //     return n;

    uint64_t nodes = 0;
    for (std::size_t i = 0; i < n; ++i)
    {
        const auto move = moves[i];
        pos.makemove(move, setup);
        
        if (pos.inCheck(pos.turn().prev()))
        {
            pos.undomove(move, setup);
            continue;
        }

        nodes += perft(pos, depth - 1, setup);
        pos.undomove(move, setup);
    }

    return nodes;
}

// void perft(Position &pos, Record &rc, int depth, int maxDepth, bool full = false)
// {
//     const GS&
//     curr = pos.states[depth + 0],
//     next = pos.states[depth + 1];

//     if (depth >= maxDepth)
//     {
//         rc.nodes++;
//         return;
//     }

//     MoveList moves;
//     moves.genNoisyMoves(pos, Modern, depth);
//     moves.genQuietMoves(pos, Modern, depth); //std::cout << "X" << moves.size << std::endl;

//     // Move moves[MAX_MOVES];
//     // int size = 0;
//     // size += gen_all_pseudo_legal_moves<Noisy>(pos, moves + size);
//     // size += gen_all_pseudo_legal_moves<Quiet>(pos, moves + size);

//     // if (depth == (maxDepth - 1))
//     //     rc.nodes += moves.size;

//     bool noLegalMove = true;
//     for (int i = 0; i < moves.size; ++i)
//     {
//         pos.makemove(moves[i]);

//         // std::cout << "X " << pos.isRoyalSafe(gs.turn, depth + 1) << std::endl;
//         // std::cout << "Y " << !pos.isRoyalSafe(gs.turn, depth + 1) << std::endl;

//         // std::cout << "gs.turn " << (int)gs.turn << std::endl;

//         // std::cout << "royal sq: " << (int)pos.states[depth + 1].royals[gs.turn] << std::endl;

    
//         if (pos.isSquareAttacked(next.royals[curr.turn], curr.turn))
//         {
//             pos.undomove(moves[i]);
//             continue;
//         }

//         perft(pos, rc, depth + 1, maxDepth, full);

//         // if (isRoyalSafe(pos, gs.turn))
//         // {
//         //     noLegalMove = false;

//         //     if (depth == 1)
//         //     {
//         //         rc.nodes++;

//         //         if (full)
//         //         {
//         //             auto flag = moves[i].flag();
//         //             switch (flag)
//         //             {
//         //             case Noisy:
//         //                 rc.noisy++;
//         //                 break;
//         //             case Quiet:
//         //                 rc.quiet++;
//         //                 break;
//         //             }

//         //             auto nature = moves[i].nature();
//         //             switch (nature)
//         //             {
//         //             case Jumper:
//         //                 rc.jumper++;
//         //                 break;
//         //             case Slider:
//         //                 rc.slider++;
//         //                 break;
//         //             case Pushed:
//         //                 rc.pushed++;
//         //                 break;
//         //             case Stride:
//         //                 rc.stride++;
//         //                 break;
//         //             case Strike:
//         //                 rc.strike++;
//         //                 break;
//         //             case Evolve:
//         //                 rc.evolve++;
//         //                 break;
//         //             case Enpass:
//         //                 rc.enpass++;
//         //                 break;
//         //             case Castle:
//         //                 rc.castle++;
//         //                 break;
//         //             }
//         //         }
//         //     }

//         //     else
//         //         perft(pos, rc, depth - 1, full);
//         // }

//         pos.undomove(moves[i]);
//     }

//     // if (full && noLegalMove)
//     // {
//     //     rc.nodes += 1;
//     //     if (isRoyalSafe(pos, gs.turn))
//     //         rc.stalemates += 1;
//     //     else
//     //         rc.checkmates += 1;
//     // }
// }

// void Perft::run(Position &pos, int depth, bool full, bool split, bool cumulative)
// {
//     std::cout << std::endl;

//     if (split)
//     {
//         MoveList moves;
//         moves.genNoisyMoves(pos, Modern, depth);
//         moves.genQuietMoves(pos, Modern, depth);

//         uint64_t totalNodes = 0;
//         double totalTime = 0.0;

//         for (int i = 0; i < moves.size; ++i)
//         {
//             PS x = pos;
//             x.makemove(moves[i]);

//             // x.print();

//             const GS&
//             curr = x.states[depth + 0],
//             next = x.states[depth + 1];

//             if (x.isSquareAttacked(next.royals[curr.turn], curr.turn))
//             {
//                 x.undomove(moves[i]);
//                 continue;
//             }

//             Record rc;

//             auto start = std::chrono::high_resolution_clock::now();
//             perft(x, rc, 1, depth, false);
//             auto end = std::chrono::high_resolution_clock::now();

//             std::chrono::duration<double> elapsed = end - start;
//             double seconds = elapsed.count();

//             std::cout << moves[i].uci(true) << ": " << rc.nodes << std::endl;

//             totalNodes += rc.nodes;
//             totalTime += seconds;

//             x.undomove(moves[i]);
//         }

//         std::cout << "total: " << formatInt(totalNodes) << " nodes, "
//                   << std::fixed << std::setprecision(6) << totalTime << " s, "
//                   << formatInt(static_cast<uint64_t>(totalNodes / totalTime)) << " nps\n";
//     }
//     else
//     {
//         if (full)
//         {
//             std::cout << std::setw(6) << "depth"
//                         << std::setw(12) << "nodes"
//                         << std::setw(10) << "quiet"
//                         << std::setw(10) << "noisy"
//                         << std::setw(10) << "jumper"
//                         << std::setw(10) << "slider"
//                         << std::setw(10) << "pushed"
//                         << std::setw(10) << "stride"
//                         << std::setw(10) << "strike"
//                         << std::setw(10) << "evolve"
//                         << std::setw(10) << "enpass"
//                         << std::setw(10) << "castle"
//                         << std::setw(12) << "stalemates"
//                         << std::setw(13) << "checkmates"
//                         << std::setw(12) << "time(s)"
//                         << std::setw(15) << "nps" << "\n";
//         }
//         else
//         {
//             std::cout << std::setw(10) << "depth"
//                         << std::setw(15) << "nodes"
//                         << std::setw(15) << "time (s)"
//                         << std::setw(15) << "nps" << "\n";
//         }

//         std::cout << std::string(full ? 185 : 55, '-') << "\n";

//         Record cumulative_rc;
//         Record total_rc;
//         double totalTime = 0.0;

//         for (int d = 1; d <= depth; ++d)
//         {
//             PS x = pos;
//             Record rc;

//             auto start = std::chrono::high_resolution_clock::now();
//             perft(x, rc, 0, d, full);
//             auto end = std::chrono::high_resolution_clock::now();

//             std::chrono::duration<double> elapsed = end - start;
//             double seconds = elapsed.count();

//             if (cumulative)
//                 cumulative_rc += rc;
//             else
//                 cumulative_rc = rc;

//             total_rc += cumulative_rc;
//             totalTime += seconds;

//             uint64_t nodes = cumulative ? cumulative_rc.nodes : rc.nodes;
//             uint64_t nps = static_cast<uint64_t>(nodes / totalTime);

//             if (full)
//             {
//                 std::cout << std::setw(6) << d
//                             << std::setw(12) << formatInt(nodes)
//                             << std::setw(10) << formatInt(cumulative_rc.quiet)
//                             << std::setw(10) << formatInt(cumulative_rc.noisy)
//                             << std::setw(10) << formatInt(cumulative_rc.jumper)
//                             << std::setw(10) << formatInt(cumulative_rc.slider)
//                             << std::setw(10) << formatInt(cumulative_rc.pushed)
//                             << std::setw(10) << formatInt(cumulative_rc.stride)
//                             << std::setw(10) << formatInt(cumulative_rc.strike)
//                             << std::setw(10) << formatInt(cumulative_rc.evolve)
//                             << std::setw(10) << formatInt(cumulative_rc.enpass)
//                             << std::setw(10) << formatInt(cumulative_rc.castle)
//                             << std::setw(12) << formatInt(cumulative_rc.stalemates)
//                             << std::setw(13) << formatInt(cumulative_rc.checkmates)
//                             << std::setw(12) << std::fixed << std::setprecision(6) << totalTime
//                             << std::setw(15) << formatInt(nps) << "\n";
//             }
//             else
//             {
//                 std::cout << std::setw(10) << d
//                             << std::setw(15) << formatInt(nodes)
//                             << std::setw(15) << std::fixed << std::setprecision(6) << totalTime
//                             << std::setw(15) << formatInt(nps) << "\n";
//             }
//         }

//         std::cout << std::string(full ? 185 : 55, '-') << "\n";

//         if (full)
//         {
//             std::cout << std::setw(6) << "total"
//                         << std::setw(12) << formatInt(total_rc.nodes)
//                         << std::setw(10) << formatInt(total_rc.quiet)
//                         << std::setw(10) << formatInt(total_rc.noisy)
//                         << std::setw(10) << formatInt(total_rc.jumper)
//                         << std::setw(10) << formatInt(total_rc.slider)
//                         << std::setw(10) << formatInt(total_rc.pushed)
//                         << std::setw(10) << formatInt(total_rc.stride)
//                         << std::setw(10) << formatInt(total_rc.strike)
//                         << std::setw(10) << formatInt(total_rc.evolve)
//                         << std::setw(10) << formatInt(total_rc.enpass)
//                         << std::setw(10) << formatInt(total_rc.castle)
//                         << std::setw(12) << formatInt(total_rc.stalemates)
//                         << std::setw(13) << formatInt(total_rc.checkmates)
//                         << std::setw(12) << std::fixed << std::setprecision(6) << totalTime
//                         << std::setw(15) << formatInt(static_cast<uint64_t>(total_rc.nodes / totalTime)) << "\n";
//         }
//         else
//         {
//             std::cout << std::setw(10) << "total"
//                         << std::setw(15) << formatInt(total_rc.nodes)
//                         << std::setw(15) << std::fixed << std::setprecision(6) << totalTime
//                         << std::setw(15) << formatInt(static_cast<uint64_t>(total_rc.nodes / totalTime)) << "\n";
//         }
//     }

//     std::cout << std::endl;
// }














// // void runPerftTests(Position &pos, int depth, bool full, bool split, bool cumulative)
// // {
// //     std::cout << std::endl;

// //     if (split)
// //     {
// //         // Move moves[MAX_MOVES];
// //         // int size = 0;
// //         // size += genAllNoisyMoves(pos, moves + size);
// //         // size += genAllQuietMoves(pos, moves + size);

// //         // uint64_t totalNodes = 0;
// //         // double totalTime = 0.0;

// //         // for (int i = 0; i < size; ++i)
// //         // {
// //         //     pos.makemove(moves[i]);

// //         //     const GameState &gs = pos.states.back();

// //         //     if (!isRoyalSafe(pos, gs.turn))
// //         //     {
// //         //         pos.undomove(moves[i]);
// //         //         continue;
// //         //     }

// //         //     Record rc;

// //         //     auto start = std::chrono::high_resolution_clock::now();
// //         //     perft(pos, rc, depth - 1, false);
// //         //     auto end = std::chrono::high_resolution_clock::now();

// //         //     std::chrono::duration<double> elapsed = end - start;
// //         //     double seconds = elapsed.count();

// //         //     std::cout << toString(moves[i]) << ": " << rc.nodes << std::endl;

// //         //     totalNodes += rc.nodes;
// //         //     totalTime += seconds;

// //         //     pos.undomove(moves[i]);
// //         // }

// //         // std::cout << "total: " << formatInt(totalNodes) << " nodes, "
// //         //           << std::fixed << std::setprecision(6) << totalTime << " s, "
// //         //           << formatInt(static_cast<uint64_t>(totalNodes / totalTime)) << " nps\n";
// //     }
// //     else
// //     {
// //         if (full)
// //         {
// //             std::cout << std::setw(6) << "depth"
// //                         << std::setw(12) << "nodes"
// //                         << std::setw(10) << "quiet"
// //                         << std::setw(10) << "noisy"
// //                         << std::setw(10) << "jumper"
// //                         << std::setw(10) << "slider"
// //                         << std::setw(10) << "pushed"
// //                         << std::setw(10) << "stride"
// //                         << std::setw(10) << "strike"
// //                         << std::setw(10) << "evolve"
// //                         << std::setw(10) << "enpass"
// //                         << std::setw(10) << "castle"
// //                         << std::setw(12) << "stalemates"
// //                         << std::setw(13) << "checkmates"
// //                         << std::setw(12) << "time(s)"
// //                         << std::setw(15) << "nps" << "\n";
// //         }
// //         else
// //         {
// //             std::cout << std::setw(10) << "depth"
// //                         << std::setw(15) << "nodes"
// //                         << std::setw(15) << "time (s)"
// //                         << std::setw(15) << "nps" << "\n";
// //         }

// //         std::cout << std::string(full ? 185 : 55, '-') << "\n";

// //         Record cumulative_rc;
// //         Record total_rc;
// //         double totalTime = 0.0;

// //         for (int d = 1; d <= depth; ++d)
// //         {
// //             Record rc;

// //             auto start = std::chrono::high_resolution_clock::now();
// //             perft(pos, rc, d, full);
// //             auto end = std::chrono::high_resolution_clock::now();

// //             std::chrono::duration<double> elapsed = end - start;
// //             double seconds = elapsed.count();

// //             if (cumulative)
// //                 cumulative_rc += rc;
// //             else
// //                 cumulative_rc = rc;

// //             total_rc += cumulative_rc;
// //             totalTime += seconds;

// //             uint64_t nodes = cumulative ? cumulative_rc.nodes : rc.nodes;
// //             uint64_t nps = static_cast<uint64_t>(nodes / totalTime);

// //             if (full)
// //             {
// //                 std::cout << std::setw(6) << d
// //                             << std::setw(12) << formatInt(nodes)
// //                             << std::setw(10) << formatInt(cumulative_rc.quiet)
// //                             << std::setw(10) << formatInt(cumulative_rc.noisy)
// //                             << std::setw(10) << formatInt(cumulative_rc.jumper)
// //                             << std::setw(10) << formatInt(cumulative_rc.slider)
// //                             << std::setw(10) << formatInt(cumulative_rc.pushed)
// //                             << std::setw(10) << formatInt(cumulative_rc.stride)
// //                             << std::setw(10) << formatInt(cumulative_rc.strike)
// //                             << std::setw(10) << formatInt(cumulative_rc.evolve)
// //                             << std::setw(10) << formatInt(cumulative_rc.enpass)
// //                             << std::setw(10) << formatInt(cumulative_rc.castle)
// //                             << std::setw(12) << formatInt(cumulative_rc.stalemates)
// //                             << std::setw(13) << formatInt(cumulative_rc.checkmates)
// //                             << std::setw(12) << std::fixed << std::setprecision(6) << totalTime
// //                             << std::setw(15) << formatInt(nps) << "\n";
// //             }
// //             else
// //             {
// //                 std::cout << std::setw(10) << d
// //                             << std::setw(15) << formatInt(nodes)
// //                             << std::setw(15) << std::fixed << std::setprecision(6) << totalTime
// //                             << std::setw(15) << formatInt(nps) << "\n";
// //             }
// //         }

// //         std::cout << std::string(full ? 185 : 55, '-') << "\n";

// //         if (full)
// //         {
// //             std::cout << std::setw(6) << "total"
// //                         << std::setw(12) << formatInt(total_rc.nodes)
// //                         << std::setw(10) << formatInt(total_rc.quiet)
// //                         << std::setw(10) << formatInt(total_rc.noisy)
// //                         << std::setw(10) << formatInt(total_rc.jumper)
// //                         << std::setw(10) << formatInt(total_rc.slider)
// //                         << std::setw(10) << formatInt(total_rc.pushed)
// //                         << std::setw(10) << formatInt(total_rc.stride)
// //                         << std::setw(10) << formatInt(total_rc.strike)
// //                         << std::setw(10) << formatInt(total_rc.evolve)
// //                         << std::setw(10) << formatInt(total_rc.enpass)
// //                         << std::setw(10) << formatInt(total_rc.castle)
// //                         << std::setw(12) << formatInt(total_rc.stalemates)
// //                         << std::setw(13) << formatInt(total_rc.checkmates)
// //                         << std::setw(12) << std::fixed << std::setprecision(6) << totalTime
// //                         << std::setw(15) << formatInt(static_cast<uint64_t>(total_rc.nodes / totalTime)) << "\n";
// //         }
// //         else
// //         {
// //             std::cout << std::setw(10) << "total"
// //                         << std::setw(15) << formatInt(total_rc.nodes)
// //                         << std::setw(15) << std::fixed << std::setprecision(6) << totalTime
// //                         << std::setw(15) << formatInt(static_cast<uint64_t>(total_rc.nodes / totalTime)) << "\n";
// //         }
// //     }

// //     std::cout << std::endl;
// // }

} // namespace athena