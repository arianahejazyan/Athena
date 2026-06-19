#include <gtest/gtest.h>
#include <cstdint>
#include "chess/castle.h"
#include "chess/position.h"
#include "perft.h"

using namespace athena;

constexpr auto modern  = chess::Castle::Setup::Modern;
constexpr auto classic = chess::Castle::Setup::Classic;

struct TestCase {
    int depth;
    uint64_t nodes;
    std::string fen;
    chess::Castle::Setup setup;
};

inline const auto modern_fen = chess::Position::startpos(modern);
inline const std::array<TestCase, 7> tests_modern = {{
    {1, 20        , modern_fen, modern},
    {2, 395       , modern_fen, modern},
    {3, 7800      , modern_fen, modern},
    {4, 152050    , modern_fen, modern},
    {5, 3452310   , modern_fen, modern},
    {6, 77430383  , modern_fen, modern},
    {7, 1735784286, modern_fen, modern},
}};

TEST(PerftTest, Modern) {
    chess::Position pos;
    for (auto test: tests_modern) {
        pos.set_setup(test.setup);
        pos.init(test.fen);
        EXPECT_EQ(tests::run_perft_tests(pos, test.depth, false), test.nodes);
    }
}