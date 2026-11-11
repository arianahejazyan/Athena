#pragma once

#include <vector>
#include <string>
#include "chess/position.h"
#include "nnue/nnue.h"
#include "nnue/weights.h"
#include "search.h"

namespace athena {

class Engine {
public:
    Engine();

    void go(Search::Limits&);
    void stop();

    bool load(const std::string& file) noexcept { return nnue::Weights::load(file, nnue_.network()); }
    bool save(const std::string& file) noexcept { return nnue::Weights::save(file, nnue_.network()); }

    auto evaluate() noexcept {
        return nnue_.evaluate(pos_);
    }

    void set_position(const chess::Position::FEN& fen, const std::vector<std::string>& moves);

    void print(bool board16x16 = false) { pos_.print(board16x16); }
    void perft(int depth);

private:
    chess::Position pos_;
    Search search_;
    nnue::NNUE nnue_;
};

} // namespace athena
