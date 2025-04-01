#include <gtest/gtest.h>
#include "fen.h"
#include "move.h"
#include "position.h"
#include "perft.h"

using namespace std;
using namespace Talia;

class TestCase
{
    public:

        string fen;
        vector<int> expected;
};

class PerftTest : public ::testing::Test
{
    protected:

        Position pos;

        void checkMoves(string fen, vector<int> expected)
        {
            int depth = 1;
            for (const auto nodes: expected)
            {
                pos = fenToPosition(fen);

                int result = perft(pos, depth++);

                ASSERT_EQ(result, nodes);
            }
        }
};

TEST_F(PerftTest, RunAllTests)
{
    // https://www.chessprogramming.org/Perft_Results

    const vector<TestCase> testCases =
    {
        { "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", {20, 400, 8902, 197281, 4865609, 119060324}},           // Position 1
        { "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -  0 1", {48, 2039, 97862, 4085603, 193690690}},    // Position 2
        { "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", {14, 191, 2812, 43238, 674624, 11030083}},                             // Position 3
        { "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", {6, 264, 9467, 422333, 15833292}},              // Position 4
        { "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", {44, 1486, 62379, 2103487, 89941194}},                 // Position 5
        { "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10", {46, 2079, 89890, 3894594, 164075551}}, // Position 6
    };

    // Run all test cases
    for (const auto& tc : testCases) checkMoves(tc.fen, tc.expected);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}