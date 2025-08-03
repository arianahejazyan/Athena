#include <stdexcept>
#include <cstring>
#include <iostream>
#include "utility.h"

namespace athena
{

constexpr char SQUARE_TABLE[SQUARE_NB][4] =
{
    "a1" , "b1" , "c1" , "d1" , "e1" , "f1",  "g1",  "h1" , "i1" , "j1" , "k1" , "l1" , "m1" , "n1" , "o1" , "p1" ,
    "a2" , "b2" , "c2" , "d2" , "e2" , "f2",  "g2",  "h2" , "i2" , "j2" , "k2" , "l2" , "m2" , "n2" , "o2" , "p2" ,
    "a3" , "b3" , "c3" , "d3" , "e3" , "f3",  "g3",  "h3" , "i3" , "j3" , "k3" , "l3" , "m3" , "n3" , "o3" , "p3" ,
    "a4" , "b4" , "c4" , "d4" , "e4" , "f4",  "g4",  "h4" , "i4" , "j4" , "k4" , "l4" , "m4" , "n4" , "o4" , "p4" ,
    "a5" , "b5" , "c5" , "d5" , "e5" , "f5",  "g5",  "h5" , "i5" , "j5" , "k5" , "l5" , "m5" , "n5" , "o5" , "p5" ,
    "a6" , "b6" , "c6" , "d6" , "e6" , "f6",  "g6",  "h6" , "i6" , "j6" , "k6" , "l6" , "m6" , "n6" , "o6" , "p6" ,
    "a7" , "b7" , "c7" , "d7" , "e7" , "f7",  "g7",  "h7" , "i7" , "j7" , "k7" , "l7" , "m7" , "n7" , "o7" , "p7" ,
    "a8" , "b8" , "c8" , "d8" , "e8" , "f8",  "g8",  "h8" , "i8" , "j8" , "k8" , "l8" , "m8" , "n8" , "o8" , "p8" ,
    "a9" , "b9" , "c9" , "d9" , "e9" , "f9",  "g9",  "h9" , "i9" , "j9" , "k9" , "l9" , "m9" , "n9" , "o9" , "p9" ,
    "a10", "b10", "c10", "d10", "e10", "f10", "g10", "h10", "i10", "j10", "k10", "l10", "m10", "n10", "o10", "p10",
    "a11", "b11", "c11", "d11", "e11", "f11", "g11", "h11", "i11", "j11", "k11", "l11", "m11", "n11", "o11", "p11",
    "a12", "b12", "c12", "d12", "e12", "f12", "g12", "h12", "i12", "j12", "k12", "l12", "m12", "n12", "o12", "p12",
    "a13", "b13", "c13", "d13", "e13", "f13", "g13", "h13", "i13", "j13", "k13", "l13", "m13", "n13", "o13", "p13",
    "a14", "b14", "c14", "d14", "e14", "f14", "g14", "h14", "i14", "j14", "k14", "l14", "m14", "n14", "o14", "p14",
    "a15", "b15", "c15", "d15", "e15", "f15", "g15", "h15", "i15", "j15", "k15", "l15", "m15", "n15", "o15", "p15",
    "a16", "b16", "c16", "d16", "e16", "f16", "g16", "h16", "i16", "j16", "k16", "l16", "m16", "n16", "o16", "p16",
};

// size_t tokenize(char* line, char* argv[], int max_args)
// {
//     size_t argc = 0;
//     char* token = std::strtok(line, " \t\n");
//     while (token && argc < max_args)
//         argv[argc++] = token, token = std::strtok(nullptr, " \t\n");
//     return argc;
// }

std::size_t tokenize(char* line, char* argv[], int max_args, const char* delim)
{
    std::size_t argc = 0;
    char* token = std::strtok(line, delim);
    while (token && argc < max_args)
        argv[argc++] = token, token = std::strtok(nullptr, delim);
    return argc;
}

Piece toPiece(char piece)
{
    switch (std::tolower(piece))
    {
    case 'p': return Piece::P;
    case 'n': return Piece::N;
    case 'b': return Piece::B;
    case 'r': return Piece::R;
    case 'q': return Piece::Q;
    case 'k': return Piece::K;
    default : throw std::invalid_argument("info string invalid piece character");
    }
}

Color toColor(char color)
{
    switch (std::tolower(color))
    {
    case 'r': return Color::r;
    case 'b': return Color::b;
    case 'y': return Color::y;
    case 'g': return Color::g;
    default : throw std::invalid_argument("info string invalid color character");
    }
}

SQ toSQ(char* sq)
{
    int r = std::isdigit(sq[3]) ? ((sq[2] - '0') * 10 + (sq[3] - '0')) : (sq[2] - '0') ;
    int f = (sq[1] - 'a' + 1);
    return SQ(r, f);
}

char* write(char* out, Rights rights, Side side)
{
    *(out++) = rights(Color::r, side) ? '1' : '0'; *(out++) = ',';
    *(out++) = rights(Color::b, side) ? '1' : '0'; *(out++) = ',';
    *(out++) = rights(Color::y, side) ? '1' : '0'; *(out++) = ',';
    *(out++) = rights(Color::g, side) ? '1' : '0'; return out;
}

// char* write(char* out, SQ sq, bool debug)
// {
//     if (sq.encoded == SQ::OFFBOARD)
//     {
//         *(out++) = '-'; 
//         return out;
//     }

//     write(out, SQUARE_TABLE[debug ? sq.encoded : (sq.encoded - 17)]);
// }


char* toString(char* c_str, Piece piece, bool lowercase)
{
    switch (piece.encoded)
    {
    case Piece::P : *(c_str++) = lowercase ? 'p' : 'P'; break;
    case Piece::N : *(c_str++) = lowercase ? 'n' : 'N'; break;
    case Piece::B : *(c_str++) = lowercase ? 'b' : 'B'; break;
    case Piece::R : *(c_str++) = lowercase ? 'r' : 'R'; break;
    case Piece::Q : *(c_str++) = lowercase ? 'q' : 'Q'; break;
    case Piece::K : *(c_str++) = lowercase ? 'k' : 'K'; break;
    default: break;
    }
    return c_str;
}

const char* toString(SQ sq, bool debug) {
    return SQUARE_TABLE[debug ? sq.encoded : (sq.encoded - 17)];
}



// std::string toString(const Position& pos)
// {
//     const GameState& gs = pos.states.back();

//     std::string tn = "";
//     tn += toString(pos.setup) + " ";
//     tn += toString(gs.turn)   + " ";
//     tn += toString(gs.clock)  + " ";
//     tn += toString(gs.castle, pos.setup, KingSide)  + " ";
//     tn += toString(gs.castle, pos.setup, QueenSide) + " ";
//     tn += toString(gs.enpass) + " ";

//     int counter = 0;
//     for (auto sq: VALID_SQUARES)
//     {
//         const auto& pc = pos.board[sq];
//         if (pc == EMPTY)
//         {
//             ++counter;
//             continue;
//         }

//         if (counter > 0)
//         {
//             tn += std::to_string(counter) + ",";
//             counter = 0;
//         }

//         else tn += toString(pc) + ",";
//     }

//     if (counter > 0)
//         tn += std::to_string(counter);

//     if (tn.back() == ',')
//         tn.pop_back();

//     return tn;
// }

// void fromString(const std::string& str, Position& pos)
// {
//     pos.states.clear();
//     pos.states.reserve(256);

//     // <GameSetup> <Turn> <Clock> <KingSide> <QueenSide> <Enpassant> <Board>
//     auto tokens = tokenize(str, ' ');

//     // Parsing <GameSetup>
//     fromString(tokens[0], pos.setup);

//     // Parsing <Turn>
//     Color turn;
//     fromString(tokens[1], turn);

//     // Parsing <Clock>
//     int clock;
//     fromString(tokens[2], clock);

//     // Parsing <KingSide> <QueenSide>
//     BitBoard castle;
//     fromString(tokens[3], castle, pos.setup, KingSide);
//     fromString(tokens[4], castle, pos.setup, QueenSide);

//     // Parsing <Enpassant>
//     ndarray<Square, COLOR_NB - 1> enpass;
//     fromString(tokens[5], enpass);

//     // Parsing <Board>
//     auto board = tokenize(tokens[6], ',');
//     pos.board.clear();
//     int idx = 0;
//     for (auto sq : board)
//     {
//         if (!std::isdigit(sq[0]))
//         {
//             PieceClass pc;
//             fromString(sq, pc);
//             pos.board.setSQ(VALID_SQUARES[idx++], pc);
//         }
//         // else idx += std::stoi(sq);
//         else {
//             int skip = std::stoi(sq);
//             for (int i = 0; i < skip; ++i)
//                 pos.board.setSQ(VALID_SQUARES[idx++], EMPTY);
//         }
//     }

//     pos.states.emplace_back(clock, turn, BB(), castle, EMPTY, enpass);
// }

} // namespace athena