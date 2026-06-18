#pragma once

#include <chrono>
#include <cstdint>
#include "chess/position.h"
#include "chess/movegen.h"

namespace athena::tests {

inline std::string format(std::uint64_t num) {
    std::string s = std::to_string(num);
    for (int i = static_cast<int>(s.size()) - 3; i > 0; i -= 3) s.insert(i, ",");
    return s;
}  

inline uint64_t perft(chess::Position& pos, int depth) {

    if (depth == 0) return 1;

    chess::Move moves[chess::MOVE_NB];
    int num_moves = generate_legal_moves(pos, moves);

    if (depth == 1) return num_moves;

    uint64_t nodes = 0;
    for (int i = 0; i < num_moves; i++) {
        const auto& move = moves[i];
        pos.make_move(move);
        nodes += perft(pos, depth - 1);
        pos.undo_move(move);
    }

    return nodes;
}

inline uint64_t run_perft_tests(
    chess::Position& pos, int depth, bool split) noexcept {
    uint64_t nodes = 0, count = 0;

    if (split) {
        double seconds = 0;
        auto tic = std::chrono::high_resolution_clock::now();

        chess::Move moves[chess::MOVE_NB];
        int num_moves = generate_legal_moves(pos, moves);

        for (int i = 0; i < num_moves; i++) {
            const auto& move = moves[i];
            pos.make_move(move);
            count = perft(pos, depth - 1);
            pos.undo_move(move);
            nodes += count;
            std::cout << move.uci() << ": " << count << "\n";
        }

        auto toc = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> elapsed = toc - tic;
        seconds += elapsed.count();

        std::cout << "nodes: " << format(nodes) << " "
                  << "nps: "   << format(static_cast<uint64_t>(nodes / seconds)) << std::endl; 

    } else {
        
        auto tic = std::chrono::high_resolution_clock::now();
        nodes = perft(pos, depth);
        auto toc = std::chrono::high_resolution_clock::now();
    
        std::chrono::duration<double> elapsed = toc - tic;
        double seconds = elapsed.count();
    
        std::cout << "nodes: " << format(nodes) << " "
                  << "nps: "   << format(static_cast<uint64_t>(nodes / seconds)) << std::endl;            
    }

    return nodes;
}

} // namespace athena