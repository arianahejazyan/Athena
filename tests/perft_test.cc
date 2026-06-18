#include <gtest/gtest.h>
#include <cstdint>
#include "chess/castle.h"
#include "chess/position.h"
#include "perft.h"

using namespace athena;

#define MODERN  chess::Castle::Setup::Modern
#define CLASSIC chess::Castle::Setup::Classic
struct Test {
    int depth;
    uint64_t nodes;
    std::string fen;
    chess::Castle::Setup setup;
};

inline const std::array<Test, 7> tests_modern = {{
    {1, 20        , chess::Position::startpos(MODERN), MODERN},
    {2, 395       , chess::Position::startpos(MODERN), MODERN},
    {3, 7800      , chess::Position::startpos(MODERN), MODERN},
    {4, 152050    , chess::Position::startpos(MODERN), MODERN},
    {5, 3452310   , chess::Position::startpos(MODERN), MODERN},
    {6, 77430383  , chess::Position::startpos(MODERN), MODERN},
    {7, 1735784286, chess::Position::startpos(MODERN), MODERN},
}};

TEST(PerftTest, Modern) {
    const auto modern = chess::Castle::Setup::Modern ;
    chess::Position pos;
    for (auto test: tests_modern) {
        pos.set_setup(test.setup);
        pos.init(test.fen);
        EXPECT_EQ(tests::run_perft_tests(pos, test.depth, false), test.nodes);
    }
}