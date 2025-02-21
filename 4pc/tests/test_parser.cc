#include <gtest/gtest.h>
#include "parser.h"

using namespace parser;

class ParserTest: public ::testing::Test
{
    protected:
    
        int digit;

        int integer;

        Position pos;

        Player player;

        Piece piece;

        Square sq;

        
        

        string fen1 = "R-0,0,0,0-1,1,1,1-1,1,1,1-0,0,0,0-0-x,x,x,yR,yN,yB,yK,yQ,yB,yN,yR,x,x,x/x,x,x,yP,yP,yP,yP,yP,1,yP,yP,x,x,x/x,x,x,8,x,x,x/bR,bP,10,gP,gR/bN,bP,6,yP,3,gP,gN/bB,bP,10,gP,gB/bK,bP,10,gP,gQ/bQ,bP,10,gP,gK/bB,bP,10,gP,gB/bN,bP,10,gP,gN/bR,bP,10,gP,gR/x,x,x,8,x,x,x/x,x,x,rP,rP,rP,rP,rP,rP,rP,rP,x,x,x/x,x,x,rR,rN,rB,rQ,rK,rB,rN,rR,x,x,x";
        string fen2 = "B-0,0,0,0-1,1,1,1-1,1,1,1-0,0,0,0-0-{'enPassant':('d3:d4','d3:d4','d3:d4','d3:d4')}-x,x,x,yR,yN,yB,yK,yQ,yB,yN,yR,x,x,x/x,x,x,yP,yP,yP,yP,yP,yP,yP,yP,x,x,x/x,x,x,8,x,x,x/bR,bP,10,gP,gR/bN,bP,10,gP,gN/bB,bP,10,gP,gB/bQ,bP,10,gP,gK/bK,bP,10,gP,gQ/bB,bP,10,gP,gB/bN,bP,10,gP,gN/bR,bP,1,rP,8,gP,gR/x,x,x,8,x,x,x/x,x,x,1,rP,rP,rP,rP,rP,rP,rP,x,x,x/x,x,x,rR,rN,rB,rQ,rK,rB,rN,rR,x,x,x";
};

TEST_F(ParserTest, Lower)
{
    EXPECT_EQ(lower('B'), 'b');
    EXPECT_EQ(lower('r'), 'r');
    EXPECT_EQ(lower('@'), '@');
    EXPECT_EQ(lower(' '), ' ');
}

TEST_F(ParserTest, Substring)
{
    EXPECT_EQ(substring("z6%(#)@(#)*&%", '(', ')'), "#)@(#");
    EXPECT_EQ(substring("$8", '$', '*'), "");
    EXPECT_EQ(substring("#w@", '@', '#'), "");
}

TEST_F(ParserTest, SplitString)
{
    EXPECT_EQ(split_string(",32, one,, @ ,x2z,,", ','), vector<string>({"", "32", " one", "", " @ ", "x2z", "", ""}));
}

TEST_F(ParserTest, SplitBoard)
{
    EXPECT_EQ(split_board("x,@ ,rP/2, /bQ,x,,1,"), vector<string>({"bQ", "x", "", "1", "", "2", " ", "x", "@ ", "rP"}));
}

TEST_F(ParserTest, SplitStatus)
{
    EXPECT_EQ(split_status("#:(a,b),@:(x,y)", '(', ')', ','), vector<string>({"#:(a,b)", "@:(x,y)"}));
    EXPECT_EQ(split_status("#:(a,b)", '(', ')', ':'), vector<string>({"#", "(a,b)"}));
}

TEST_F(ParserTest, ParseDigit)
{
    EXPECT_TRUE(parse_digit(digit, '7'));
    EXPECT_EQ(digit, 7);

    EXPECT_FALSE(parse_digit(digit, ' '));
    EXPECT_FALSE(parse_digit(digit, '$'));
}

TEST_F(ParserTest, ParseInteger)
{
    EXPECT_TRUE(parse_integer(integer, "12"));
    EXPECT_EQ(integer, 12);

    EXPECT_TRUE(parse_integer(integer, "+9"));
    EXPECT_EQ(integer, 9);

    EXPECT_TRUE(parse_integer(integer, "-6"));
    EXPECT_EQ(integer, -6);

    EXPECT_FALSE(parse_integer(integer, ""));
    EXPECT_FALSE(parse_integer(integer, " "));
    EXPECT_FALSE(parse_integer(integer, "@"));
    EXPECT_FALSE(parse_integer(integer, "+"));
}

TEST_F(ParserTest, ParseSquare)
{
    EXPECT_TRUE(parse_square(sq, "d1"));
    EXPECT_EQ(sq, 0);

    EXPECT_TRUE(parse_square(sq, "k14"));
    EXPECT_EQ(sq, 159);

    EXPECT_TRUE(parse_square(sq, "g7"));
    EXPECT_EQ(sq, 72);

    EXPECT_TRUE(parse_square(sq, "n7"));
    EXPECT_EQ(sq, 79);

    EXPECT_FALSE(parse_square(sq, " "));
    EXPECT_FALSE(parse_square(sq, "a1"));
    EXPECT_FALSE(parse_square(sq, "Q5"));
    EXPECT_FALSE(parse_square(sq, "B15"));
}

TEST_F(ParserTest, ParsePiece)
{
    EXPECT_TRUE(parse_piece(piece, 'Q'));
    EXPECT_EQ(piece, Queen);

    EXPECT_TRUE(parse_piece(piece, 'k'));
    EXPECT_EQ(piece, King);

    EXPECT_FALSE(parse_piece(piece, ' '));
    EXPECT_FALSE(parse_piece(piece, 'A'));
    EXPECT_FALSE(parse_piece(piece, '$'));
}

TEST_F(ParserTest, ParsePlayer)
{
    EXPECT_TRUE(parse_player(player, 'B'));
    EXPECT_EQ(player, Blue);

    EXPECT_TRUE(parse_player(player, 'g'));
    EXPECT_EQ(player, Green);

    EXPECT_FALSE(parse_player(player, ' '));
    EXPECT_FALSE(parse_player(player, '#'));  
}

TEST_F(ParserTest, ParseTurn)
{
    EXPECT_TRUE(parse_turn(pos, ""));

    EXPECT_TRUE(parse_turn(pos, "r"));
    EXPECT_EQ(pos.turn, Red);

    EXPECT_TRUE(parse_turn(pos, "Y"));
    EXPECT_EQ(pos.turn, Yellow);

    EXPECT_FALSE(parse_turn(pos, " "));
    EXPECT_FALSE(parse_turn(pos, "x"));
}

TEST_F(ParserTest, ParseState)
{
    EXPECT_TRUE(parse_state(pos, ""));

    EXPECT_TRUE(parse_state(pos, ",1,0*,,,"));
    EXPECT_EQ(pos.states[Blue], Alive);
    EXPECT_EQ(pos.states[Yellow], Dead);
    
    EXPECT_FALSE(parse_state(pos, " "));
    EXPECT_FALSE(parse_state(pos, "@"));
}

TEST_F(ParserTest, ParseRight)
{
    EXPECT_TRUE(parse_right(pos, "", KingSide));

    EXPECT_TRUE(parse_right(pos, "0,,1$,,", QueenSide));
    EXPECT_EQ(pos.rights[Red][QueenSide], false);
    EXPECT_EQ(pos.rights[Yellow][QueenSide], true);

    EXPECT_FALSE(parse_right(pos, " ", KingSide));
    EXPECT_FALSE(parse_right(pos, "@", QueenSide));
}

TEST_F(ParserTest, ParseScore)
{
    EXPECT_TRUE(parse_score(pos, ""));

    EXPECT_TRUE(parse_score(pos, "80,0,+32,-40,,#"));
    EXPECT_EQ(pos.scores[Red], 80);
    EXPECT_EQ(pos.scores[Blue], 0);
    EXPECT_EQ(pos.scores[Yellow], 32);
    EXPECT_EQ(pos.scores[Green], -40);

    EXPECT_FALSE(parse_score(pos, " "));
    EXPECT_FALSE(parse_score(pos, "$"));
}

TEST_F(ParserTest, ParseFiftyRule)
{
    EXPECT_TRUE(parse_fifty_rule(pos, "30"));
    EXPECT_EQ(pos.fifty_rule, 30);

    EXPECT_TRUE(parse_fifty_rule(pos, "+20"));
    EXPECT_EQ(pos.fifty_rule, 20);

    EXPECT_FALSE(parse_fifty_rule(pos, ""));
    EXPECT_FALSE(parse_fifty_rule(pos, " "));
    EXPECT_FALSE(parse_fifty_rule(pos, "#"));
    EXPECT_FALSE(parse_fifty_rule(pos, "-3"));
}

TEST_F(ParserTest, ParseRoyal)
{ 
    EXPECT_TRUE(parse_royal(pos, "h1,a8,g14,n7"));
    EXPECT_EQ(pos.royals[Red], 4);
    EXPECT_EQ(pos.royals[Blue], 80);
    EXPECT_EQ(pos.royals[Yellow], 155);
    EXPECT_EQ(pos.royals[Green], 79);
}

TEST_F(ParserTest, ParseEnpassant)
{
    EXPECT_TRUE(parse_enpassant(pos, "d3:d4,d3:d4,d3:d4,d3:d4"));
    EXPECT_EQ(pos.marked[Red], 16);
    EXPECT_EQ(pos.target[Red], 27);


    /*
    EXPECT_TRUE(parse_enpassant(pos, "{'enPassant':('d3:d4','','','')}"));
    EXPECT_EQ(pos.marked[Red], 16);
    EXPECT_EQ(pos.target[Red], 27);*/
}

TEST_F(ParserTest, ParseStatus)
{
    /*
    EXPECT_TRUE(parse_status(pos, "e:(c4:d4,c4:d4,c4:c5,c4:c5)"));
    EXPECT_EQ(pos.marked[Blue], 26);
    EXPECT_EQ(pos.target[Blue], 27);

    EXPECT_TRUE(parse_status(pos, "r:(g9,g9,g9,g9)"));
    EXPECT_EQ(pos.royals[Red], 100);
    */
    EXPECT_TRUE(parse_status(pos, "r:(g9,g9,g9,g9),e:(c4:d4,c4:d4,c4:c5,c4:c5)"));
    EXPECT_EQ(pos.royals[Red], 100);
    EXPECT_EQ(pos.marked[Blue], 26);
    EXPECT_EQ(pos.target[Blue], 27);

    /*
    EXPECT_TRUE(parse_status(pos, "r:(g9,,),e:(c4:c5,c4:c5,c4:c5,c4:c5)"));
    EXPECT_EQ(pos.royals[Red], 100);
    EXPECT_EQ(pos.marked[Blue], 26);
    EXPECT_EQ(pos.target[Blue], 27);*/
}

TEST_F(ParserTest, RemoveChar)
{
    EXPECT_EQ(remove_char("\'r\':(\'@\')", '\''), "r:(@)");
}

TEST_F(ParserTest, ParseBoard)
{
    EXPECT_TRUE(parse_board(pos, "2rQ3=,#,x,59,*bbN/31,x,9,,@,yP/,wK, v ,gR56,59"));
    EXPECT_EQ(pos.pieces[0], Rook);
    EXPECT_EQ(pos.players[0], Green);
    EXPECT_EQ(pos.pieces[100], Pawn);
    EXPECT_EQ(pos.players[100], Yellow);
    //EXPECT_EQ(pos.players[0], Green);
    //EXPECT_EQ(pos.pieces[159], Pawn);
    //EXPECT_EQ(pos.players[159], Red);

    /*
    string board = "2rQ3=,#,x,59,bbN/31,x,9,,@,yP/,wK, v ,gR56,59";

    EXPECT_TRUE(parse_board(pos, board));
    EXPECT_EQ(pos.pieces[0], Rook);
    EXPECT_EQ(pos.players[0], Green);
    EXPECT_EQ(pos.pieces[100], Pawn);
    EXPECT_EQ(pos.players[100], Yellow);*/
}

TEST_F(ParserTest, ParseFen)
{
    EXPECT_TRUE(parse_fen(pos, fen1));
    EXPECT_TRUE(parse_fen(pos, fen2));
}