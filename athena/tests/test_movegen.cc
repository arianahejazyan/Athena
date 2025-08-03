#include <gtest/gtest.h>
#include "position.h"
#include "move.h"
#include "utility.h"

using namespace athena;

class TestMoveGen : public ::testing::Test
{
protected:
    MoveList moves;
    Position pos;

    void checkMoves(int size, const std::vector<std::string> &expected)
    {
        ASSERT_EQ(size, expected.size()) << "Move count mismatch";
        for (int i = 0; i < size; ++i)
        {
            std::string actual = moves[i].uci(true);
            ASSERT_EQ(actual, expected[i])
                << "mismatch at index " << i << ": expected '" << expected[i]
                << "', got '" << actual << "'";
        }
    }
};

// TEST_F(TestMoveGen, IsSquareAttacked)
// {
//     char fen[1024];
//     std::strcpy(fen, "R-0,0,0,0-0,0,0,0-0,0,0,0-0,0,0,0-0-x,x,x,8,x,x,x/x,x,x,8,x,x,x/x,x,x,8,x,x,x/14/14/14/14/4,rN,9/14/14/14/x,x,x,8,x,x,x/x,x,x,8,x,x,x/x,x,x,8,x,x,x");    
//     pos.init(fen);
//     EXPECT_TRUE( (pos.isSquareAttacked<2, Color::o>(SQ::G10)));
//     EXPECT_FALSE((pos.isSquareAttacked<1, Color::o>(SQ::H8 )));
    
//     std::strcpy(fen, "R-0,0,0,0-0,0,0,0-0,0,0,0-0,0,0,0-0-x,x,x,8,x,x,x/x,x,x,8,x,x,x/x,x,x,8,x,x,x/14/14/6,rK,7/14/14/14/14/14/x,x,x,8,x,x,x/x,x,x,8,x,x,x/x,x,x,8,x,x,x");    
//     pos.init(fen);
//     EXPECT_TRUE( (pos.isSquareAttacked<2, Color::o>(SQ::G11)));
//     EXPECT_FALSE((pos.isSquareAttacked<1, Color::o>(SQ::F7 )));

//     std::strcpy(fen, "R-0,0,0,0-0,0,0,0-0,0,0,0-0,0,0,0-0-x,x,x,8,x,x,x/x,x,x,8,x,x,x/x,x,x,8,x,x,x/14/14/14/14/14/14/14/14/x,x,x,3,gP,4,x,x,x/x,x,x,8,x,x,x/x,x,x,8,x,x,x");    
//     pos.init(fen);
//     pos.print(true);
//     EXPECT_TRUE( (pos.isSquareAttacked<0, Color::c>(SQ::G3)));
//     EXPECT_FALSE((pos.isSquareAttacked<0, Color::c>(SQ::G4)));


//     std::strcpy(fen, "R-0,0,0,0-0,0,0,0-0,0,0,0-0,0,0,0-0-x,x,x,8,x,x,x/x,x,x,8,x,x,x/x,x,x,8,x,x,x/3,bR,3,bP,6/14/14/14/14/14/14/14/x,x,x,8,x,x,x/x,x,x,8,x,x,x/x,x,x,8,x,x,x");    
//     pos.init(fen);
//     pos.print(true);
//     EXPECT_TRUE( (pos.isSquareAttacked<3, Color::c>(SQ::E15)));
//     EXPECT_FALSE((pos.isSquareAttacked<2, Color::c>(SQ::M12)));


//     std::strcpy(fen, "R-0,0,0,0-0,0,0,0-0,0,0,0-0,0,0,0-0-x,x,x,8,x,x,x/x,x,x,8,x,x,x/x,x,x,8,x,x,x/14/14/14/14/14/14/14/8,gP,5/x,x,x,8,x,x,x/x,x,x,3,gB,4,x,x,x/x,x,x,8,x,x,x");    
//     pos.init(fen);
//     pos.print(true);
//     EXPECT_TRUE( (pos.isSquareAttacked<1, Color::c>(SQ::D7)));
//     EXPECT_FALSE((pos.isSquareAttacked<1, Color::c>(SQ::K6)));

// }

// TEST_F(TestMoveGen, GenKingMoves)
// {
//     char fen[1024];
//     std::strcpy(fen, "R-0,0,0,0-0,0,0,0-0,0,0,0-0,0,0,0-0-x,x,x,8,x,x,x/x,x,x,8,x,x,x/x,x,x,8,x,x,x/14/14/14/14/14/14/14/14/x,x,x,8,x,x,x/x,x,x,yP,bP,6,x,x,x/x,x,x,rK,7,x,x,x");    
//     pos.init(fen);
//     moves.size = 0;
//     moves.genNoisyMoves(pos);
//     moves.genQuietMoves(pos);
//     checkMoves(moves.size, {"e2f3", "e2f2"});
// }

// TEST_F(TestMoveGen, GenKnightMoves)
// {
//     char fen[1024];
//     std::strcpy(fen, "B-0,0,0,0-0,0,0,0-0,0,0,0-0,0,0,0-0-x,x,x,8,x,x,x/x,x,x,8,x,x,x/x,x,x,8,x,x,x/14/14/14/14/14/14/14/14/x,x,x,rP,1,gP,5,x,x,x/x,x,x,8,x,x,x/x,x,x,1,bN,6,x,x,x");    
//     pos.init(fen);
//     moves.size = 0;
//     moves.genNoisyMoves(pos);
//     moves.genQuietMoves(pos);
//     checkMoves(moves.size, {"f2e4", "f2h3"});
// }

// TEST_F(TestMoveGen, GenRookMoves)
// {
//     char fen[1024];
//     std::strcpy(fen, "Y-0,0,0,0-0,0,0,0-0,0,0,0-0,0,0,0-0-x,x,x,8,x,x,x/x,x,x,8,x,x,x/x,x,x,8,x,x,x/14/14/14/14/14/14/14/14/x,x,x,1,bR,6,x,x,x/x,x,x,8,x,x,x/x,x,x,1,yR,1,rR,4,x,x,x");    
//     pos.init(fen);
//     moves.size = 0;
//     moves.genNoisyMoves(pos);
//     moves.genQuietMoves(pos);
//     checkMoves(moves.size, {"f2f4", "f2e2", "f2g2", "f2f3"});
// }

// TEST_F(TestMoveGen, GenBishopMoves)
// {
//     char fen[1024];
//     std::strcpy(fen, "G-0,0,0,0-0,0,0,0-0,0,0,0-0,0,0,0-0-x,x,x,8,x,x,x/x,x,x,2,rB,5,x,x,x/x,x,x,8,x,x,x/3,gB,10/4,yB,9/1,bB,12/14/14/14/14/14/x,x,x,8,x,x,x/x,x,x,8,x,x,x/x,x,x,8,x,x,x");    
//     pos.init(fen);
//     moves.size = 0;
//     moves.genNoisyMoves(pos);
//     moves.genQuietMoves(pos);
//     checkMoves(moves.size, {"e12f11", "e12g14", "e12d11", "e12f13"});
// }

// TEST_F(TestMoveGen, GenPushMoves)
// {
//     char fen[1024];
//     std::strcpy(fen, "R-0,0,0,0-0,0,0,0-0,0,0,0-0,0,0,0-0-x,x,x,8,x,x,x/x,x,x,8,x,x,x/x,x,x,8,x,x,x/14/14/14/14/14/14/14/14/x,x,x,3,rP,2,yP,1,x,x,x/x,x,x,2,rP,3,rP,1,x,x,x/x,x,x,8,x,x,x");    
//     pos.init(fen);
//     moves.size = 0;
//     moves.genNoisyMoves(pos);
//     moves.genQuietMoves(pos);
//     checkMoves(moves.size, {"g3g4", "h4h5", "g3g5"});
// }

// TEST_F(TestMoveGen, GenTakeMoves)
// {
//     char fen[1024];
//     std::strcpy(fen, "R-0,0,0,0-0,0,0,0-0,0,0,0-0,0,0,0-0-x,x,x,8,x,x,x/x,x,x,8,x,x,x/x,x,x,8,x,x,x/14/14/14/14/14/14/14/14/x,x,x,1,bP,gP,3,yP,yP,x,x,x/x,x,x,1,rP,5,rP,x,x,x/x,x,x,8,x,x,x");    
//     pos.init(fen);
//     moves.size = 0;
//     moves.genNoisyMoves(pos);
//     moves.genQuietMoves(pos);
//     checkMoves(moves.size, {"f3g4"});
// }

// TEST_F(TestMoveGen, GenEvolveMoves)
// {
//     char fen[1024];
//     std::strcpy(fen, "B-0,0,0,0-0,0,0,0-0,0,0,0-0,0,0,0-0-x,x,x,6,bP,gP,x,x,x/x,x,x,8,x,x,x/x,x,x,7,gP,x,x,x/9,bP,4/10,yP,3/14/14/14/14/14/14/x,x,x,8,x,x,x/x,x,x,8,x,x,x/x,x,x,8,x,x,x");    
//     pos.init(fen);
//     moves.size = 0;
//     moves.genNoisyMoves(pos);
//     moves.genQuietMoves(pos);
//     checkMoves(moves.size, {"k12l11q", "k12l11r", "k12l11b", "k12l11n", "k12l12q", "k12l12r", "k12l12b", "k12l12n"});
// }

// TEST_F(TestMoveGen, GenEnpassMoves)
// {
//     char fen[1024];
//     std::strcpy(fen, "R-0,0,0,0-0,0,0,0-0,0,0,0-0,0,0,0-0-{'enPassant':('','','','l8:k8')}-x,x,x,8,x,x,x/x,x,x,8,x,x,x/x,x,x,8,x,x,x/14/14/14/10,gP,3/10,rP,1,rP,1/14/14/14/x,x,x,8,x,x,x/x,x,x,8,x,x,x/x,x,x,8,x,x,x");    
//     pos.init(fen);
//     moves.size = 0;
//     moves.genNoisyMoves(pos);
//     moves.genQuietMoves(pos);
//     checkMoves(moves.size, {"l8m9", "n8m9", "n8n9"});
// }

TEST_F(TestMoveGen, GenCastleMoves)
{
    char fen[1024];
    std::strcpy(fen, "B-0,0,0,0-0,0,0,0-0,0,0,0-0,0,0,0-0-x,x,x,8,x,x,x/x,x,x,8,x,x,x/x,x,x,8,x,x,x/14/14/14/14/14/14/14/14/x,x,x,8,x,x,x/x,x,x,8,x,x,x/x,x,x,8,x,x,x");    
    pos.init(fen);
    moves.size = 0;
    moves.genNoisyMoves(pos);
    moves.genQuietMoves(pos);
    checkMoves(moves.size, {});

    std::strcpy(fen, "B-0,0,0,0-0,1,0,0-0,1,0,0-0,0,0,0-0-x,x,x,8,x,x,x/x,x,x,8,x,x,x/x,x,x,8,x,x,x/14/14/14/14/14/14/14/14/x,x,x,8,x,x,x/x,x,x,8,x,x,x/x,x,x,8,x,x,x");    
    pos.init(fen);
    pos.print(true);
    moves.size = 0;
    moves.genNoisyMoves(pos);
    moves.genQuietMoves(pos);
    checkMoves(moves.size, {"b8b6", "b8b10"});


    std::strcpy(fen, "B-0,0,0,0-0,1,0,0-0,1,0,0-0,0,0,0-0-x,x,x,8,x,x,x/x,x,x,8,x,x,x/x,x,x,8,x,x,x/14/gN,13/14/14/14/14/gN,13/14/x,x,x,8,x,x,x/x,x,x,8,x,x,x/x,x,x,8,x,x,x");    
    pos.init(fen);
    pos.print(true);
    moves.size = 0;
    moves.genNoisyMoves(pos);
    moves.genQuietMoves(pos);
    checkMoves(moves.size, {});

    std::strcpy(fen, "B-0,0,0,0-0,1,0,0-0,1,0,0-0,0,0,0-0-x,x,x,8,x,x,x/x,x,x,8,x,x,x/x,x,x,8,x,x,x/14/14/14/2,yQ,11/14/14/14/14/x,x,x,8,x,x,x/x,x,x,8,x,x,x/x,x,x,8,x,x,x");    
    pos.init(fen);
    pos.print(true);
    moves.size = 0;
    moves.genNoisyMoves(pos);
    moves.genQuietMoves(pos);
    checkMoves(moves.size, {});
}