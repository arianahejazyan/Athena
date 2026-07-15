#include <chrono>
#include "chess/movegen.h"
#include "search.h"
#include "chess/position.h"
#include "eval.h"

namespace athena {

void Search::start_thinking(const chess::Position& pos, const Search::Limits& limits) {

    chess::Position curr = pos;
    Search::State ss;
    nodes = 0;
    selDepth = 0;
    auto startTime = std::chrono::steady_clock::now();

    for (int depth = 1; depth <= limits.maxDepth; ++depth) {

        Score score = search(ss, curr, depth, -SCORE_INFINITE, SCORE_INFINITE, 0);
        ss.bestScore = score;

        auto now = std::chrono::steady_clock::now();
        auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();
        uint64_t nps = (elapsedMs > 0) ? (nodes * 1000ULL / elapsedMs) : 0;

        std::cout << "info"
                  << " depth " << depth
                  << " seldepth " << selDepth
                  << " score " << (isMateScore(score) ? "mate " + std::to_string(mateIn(score)) : "cp " + std::to_string(score))
                  << " nodes " << nodes
                  << " nps " << nps
                  << " time " << elapsedMs;

        std::cout << std::endl;
    }

    std::cout << "bestmove " << ss.bestMove.uci() << std::endl;
}

Score Search::search(Search::State& ss, chess::Position& pos, int depth, Score alpha, Score beta, int ply) {

    nodes++;
    selDepth = std::max(selDepth, ply);

    if (depth <= 0) {
        return evaluate(pos);
    }

    chess::Move moves[chess::MOVE_NB];
    auto move_count = chess::generate_legal_moves(pos, moves);

    // Handle checkmate and stalemate
    if (move_count == 0) {
        return pos.incheck() ? -SCORE_MATE + ply : SCORE_DRAW;
    }

    Score bestValue = -SCORE_INFINITE;

    for (int i = 0; i < move_count; i++) {
        const auto& move = moves[i];

        pos.make_move(move);
        Score score = -search(ss, pos, depth - 1, -beta, -alpha, ply + 1);
        pos.undo_move(move);

        if (score > bestValue) {
            bestValue = score;
            // Root node
            if (ply == 0) {
                ss.bestMove = move;
                ss.bestScore = score;
            }
        }

        alpha = std::max(alpha, score);
        if (alpha >= beta) break;
    }

    return bestValue;
}

} // namespace athena