#include <chrono>
#include "engine.h"
#include "chess/position.h"
#include "chess/movegen.h"
#include "chess/perft.h"

namespace athena {

Engine::Engine() {
    pos_.init(chess::Position::startpos());
}

void Engine::go(Search::Limits& limits) {
    search_.start_thinking(pos_, limits);
}

void Engine::stop() {}
    
void Engine::set_position(const chess::Position::FEN& fen, const std::vector<std::string>& moves) {
    pos_.init(fen);
    for (auto move: moves) pos_.make_move(move);
}

void Engine::perft(int depth) {
    
    auto pos = pos_;
    uint64_t nodes = 0;

    chess::Move moves[chess::MOVE_NB];
    int num_moves = chess::generate_legal_moves(pos, moves);

    for (int i = 0; i < num_moves; ++i) {
        const auto& move = moves[i];

        pos.make_move(move);
        uint64_t count = chess::perft(pos, depth - 1);
        pos.undo_move(move);

        nodes += count;
        std::cout << move.uci() << ": " << count << '\n';
    }

    std::cout << "Nodes: " << nodes << '\n';
}

} // namespace athena