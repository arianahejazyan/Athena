#pragma once

#include <chess/position.h>
#include "chess/constants.h"
#include "score.h"

namespace athena {

class Search {
public:
    struct Limits {
    public:
        int maxDepth = chess::PLAY_NB;
    };

    struct State {
    public:
        chess::Move bestMove;
        Score bestScore;
    
        State() { reset(); }
    
        void reset() {
            bestMove = chess::Move{};
            bestScore = -SCORE_INFINITE;
        }
    };

    void start_thinking(const chess::Position&, const Search::Limits&);

private:
    uint64_t nodes = 0;
    int selDepth = 0;

    Score search(Search::State& ss, chess::Position& pos, int depth, Score alpha, Score beta, int ply);
};

} // namespace athena