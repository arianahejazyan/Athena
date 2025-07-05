#include <gtest/gtest.h>
#include "position.h"
#include "movegen.h"
#include "utility.h"

using namespace athena;

class TestMoveGen : public ::testing::Test
{
    protected:

        Move moves[MAX_MOVES];
        int size = 0;
        Position pos;
        
        void checkMoves(int size, const std::vector<std::string>& expected)
        {
            ASSERT_EQ(size, expected.size()) << "Move count mismatch";
            for (int i = 0; i < size; ++i) 
            {
                std::string actual = toString(moves[i]);
                ASSERT_EQ(actual, expected[i]) 
                << "mismatch at index " << i << ": expected '" << expected[i]
                << "', got '" << actual << "'";
            }
        }
};

TEST_F(TestMoveGen, IsSquareAttacked)
{
    // Knight
    fromString("classic r 0 0000 0000 -,-,-,- rn,159", pos);
    ASSERT_TRUE( isSquareAttacked(pos, G3, Blue)) << "Knight";
    ASSERT_FALSE(isSquareAttacked(pos, F3, Blue)) << "Knight";

    // Rook
    fromString("classic r 0 0000 0000 -,-,-,- rr,2,bp,156", pos);
    ASSERT_TRUE( isSquareAttacked(pos, G2, Blue)) << "Rook";
    ASSERT_FALSE(isSquareAttacked(pos, K2, Blue)) << "Rook";

    // Bishop
    fromString("classic r 0 0000 0000 -,-,-,- rb,17,bp,141", pos);
    ASSERT_TRUE( isSquareAttacked(pos, G4, Blue)) << "Bishop";
    ASSERT_FALSE(isSquareAttacked(pos, H5, Blue)) << "Bishop";

    // Pawn
    fromString("classic r 0 0000 0000 -,-,-,- rp,159", pos);
    ASSERT_TRUE( isSquareAttacked(pos, F3, Blue)) << "Pawn";
    ASSERT_FALSE(isSquareAttacked(pos, E3, Blue)) << "Pawn";

    // King
    fromString("classic r 0 0000 0000 -,-,-,- rk,159", pos);
    ASSERT_TRUE( isSquareAttacked(pos, F2, Blue)) << "King";
    ASSERT_FALSE(isSquareAttacked(pos, G2, Blue)) << "King";
}

TEST_F(TestMoveGen, GenJumperMoves)
{
    fromString("classic r 0 0000 0000 -,-,-,- rk,7,yp,bp,150", pos);
    size = 0; 
    size += genAllNoisyMoves(pos, moves + size);
    size += genAllQuietMoves(pos, moves + size);
    checkMoves(size, {"e2f3", "e2f2"});
}

TEST_F(TestMoveGen, GenSliderMoves)
{
    fromString("classic r 0 0000 0000 -,-,-,- rq,2,bq,12,gq,1,yq,141", pos);
    size = 0; 
    size += genAllNoisyMoves(pos, moves + size);
    size += genAllQuietMoves(pos, moves + size);
    checkMoves(size, {"e2h2", "e2e4", "e2f2", "e2g2", "e2e3", "e2f3"});
}

TEST_F(TestMoveGen, GenPushMoves)
{
    fromString("classic r 0 0000 0000 -,-,-,- rp,rp,rp,6,yp,8,yp,141", pos);
    size = 0; 
    size += genAllNoisyMoves(pos, moves + size);
    size += genAllQuietMoves(pos, moves + size);
    checkMoves(size, {"e2e3", "g2g3", "e2e4"});
}

TEST_F(TestMoveGen, GenTakeMoves)
{
    fromString("classic r 0 0000 0000 -,-,-,- rp,2,rp,4,yp,bp,1,yp,yp,147", pos);
    size = 0; 
    size += genAllNoisyMoves(pos, moves + size);
    size += genAllQuietMoves(pos, moves + size);
    checkMoves(size, {"e2f3"});
} 

TEST_F(TestMoveGen, GenEvolveMoves)
{
    fromString("classic r 0 0000 0000 -,-,-,- 108,rp,2,rp,12,yp,yp,gp,33", pos);
    size = 0; 
    size += genAllNoisyMoves(pos, moves + size);
    size += genAllQuietMoves(pos, moves + size);
    checkMoves(size, {"b11b12q", "b11b12r", "b11b12b", "b11b12n", "e11f12q", "e11f12r", "e11f12b", "e11f12n"});
}

TEST_F(TestMoveGen, GenEnpassMoves)
{
    fromString("classic r 0 0000 0000 -,d5,-,m5 16,rp,6,rp,3,bp,6,gp,125", pos);
    size = 0; 
    size += genAllNoisyMoves(pos, moves + size);
    size += genAllQuietMoves(pos, moves + size);
    checkMoves(size, {"e4d5", "l4m5"});
}

TEST_F(TestMoveGen, GenCastleMoves)
{
    fromString("classic r 0 0000 0000 -,-,-,- 160", pos);
    size = 0;
    size += genAllNoisyMoves(pos, moves + size);
    size += genAllQuietMoves(pos, moves + size);
    checkMoves(size, {});

    fromString("classic r 0 1000 1000 -,-,-,- 160", pos);
    size = 0; 
    size += genAllNoisyMoves(pos, moves + size);
    size += genAllQuietMoves(pos, moves + size);
    checkMoves(size, {"i2k2", "i2g2"});

    fromString("classic r 0 1000 1000 -,-,-,- 1,yn,158", pos);
    size = 0; 
    size += genAllNoisyMoves(pos, moves + size);
    size += genAllQuietMoves(pos, moves + size);
    checkMoves(size, {"i2k2"});

    fromString("classic r 0 1000 1000 -,-,-,- 19,gq,140", pos);
    size = 0; 
    size += genAllNoisyMoves(pos, moves + size);
    size += genAllQuietMoves(pos, moves + size);
    checkMoves(size, {});
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}