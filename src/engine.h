#pragma once

#include <vector>
#include <string>
#include "chess/position.h"
#include "search.h"

namespace athena {

class Engine {
public:
    Engine();

    void go(Search::Limits&);
    void stop();

    void set_position(const chess::Position::FEN& fen, const std::vector<std::string>& moves);

    void print(bool board16x16 = false) { pos_.print(board16x16); }
    void perft(int depth);

private:
    chess::Position pos_;
    Search search_;
};

} // namespace athena