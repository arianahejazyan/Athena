#include <gtest/gtest.h>
#include "fen.h"
#include "move.h"
#include "position.h"
#include "movegen.h"

using namespace std;
using namespace Talia;

class TestCase
{
    public:

        string fen;
        int expected;
};

class MovegenTest : public ::testing::Test
{
    protected:

        Position pos;

        void checkMoves(string fen, int expected)
        {
            pos = fenToPosition(fen);
        
            StaticArray<Move, MAX_MOVES> moves;
            int size = genAllLegalMoves(pos, moves.begin());

            ASSERT_EQ(size, expected);
        }
};

TEST_F(MovegenTest, RunAllTests)
{
    // https://www.chessprogramming.org/Perft_Results

    const vector<TestCase> testCases =
    {
        { "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 20 },                 // Position 1
        { "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -  0 1", 48 },    // Position 2
        { "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 14 },                                // Position 3
        { "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 6 },          // Position 4
        { "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", 44 },                // Position 5
        { "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10", 46 }, // Position 6
    };

    // Run all test cases
    for (const auto& tc : testCases) checkMoves(tc.fen, tc.expected);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}