#include <gtest/gtest.h>
#include "constants.h"
#include "fen.h"
#include "perft.h"
#include "position.h"

using namespace athena;

TEST(PerftTest, Modern) {
    Position pos;
    pos.init(STARTPOS);

    Perft perft;
    Perft::Options opt{};

    EXPECT_EQ(perft.perft(pos, 1, GameSetup::Modern), 20ULL);
    EXPECT_EQ(perft.perft(pos, 2, GameSetup::Modern), 395ULL);
    EXPECT_EQ(perft.perft(pos, 3, GameSetup::Modern), 7800ULL);
    EXPECT_EQ(perft.perft(pos, 4, GameSetup::Modern), 152050ULL);
}

// From Terminator
// ply=1, positions=20
// ply=2, positions=395
// ply=3, positions=7800
// ply=4, positions=152050
// ply=5, positions=3452310
// ply=6, positions=77430383
// ply=7, positions=1735784286