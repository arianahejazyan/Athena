#ifndef CHESS_HPP
#define CHESS_HPP

#include "types.hpp"

namespace ELSA
{

enum Square : u8
{
    a1, b1, c1, d1, e1, f1, g1, h1,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a8, b8, c8, d8, e8, f8, g8, h8, offboard
};

enum PieceType : u8 
{
    WPawn  , BPawn  , 
    WKnight, BKnight, 
    WBishop, BBishop, 
    WRook  , BRook  , 
    WQueen , BQueen , 
    WKing  , BKing  , none
};

enum class Piece : u8 { Pawn, Knight, Bishop, Rook, Queen, King, None };

enum class Color : u8 { White, Black, None };



enum Player : u8 {white, black};

#define next(player) (player == white ? black : white)

constexpr u32 BOARD_SIZE  = 64;
constexpr u32 PIECE_SIZE  = 13;
constexpr u32 COLOR_SIZE  = 2;
constexpr u32 CASTLE_SIZE = 2;
constexpr u32 RANK_SIZE   = 8;
constexpr u32 FILE_SIZE   = 8;

constexpr u64 RANK1 = 0x00000000000000FFull;
constexpr u64 RANK2 = 0x000000000000FF00ull;
constexpr u64 RANK3 = 0x0000000000FF0000ull;
constexpr u64 RANK4 = 0x00000000FF000000ull;
constexpr u64 RANK5 = 0x000000FF00000000ull;
constexpr u64 RANK6 = 0x0000FF0000000000ull;
constexpr u64 RANK7 = 0x00FF000000000000ull;
constexpr u64 RANK8 = 0xFF00000000000000ull;
constexpr u64 FILEA = 0x0101010101010101ull;
constexpr u64 FILEB = 0x0202020202020202ull;
constexpr u64 FILEC = 0x0404040404040404ull;
constexpr u64 FILED = 0x0808080808080808ull;
constexpr u64 FILEE = 0x1010101010101010ull;
constexpr u64 FILEF = 0x2020202020202020ull;
constexpr u64 FILEG = 0x4040404040404040ull;
constexpr u64 FILEH = 0x8080808080808080ull;

constexpr u64 WHITESIDE = 0x00000000000000FFull;
constexpr u64 BLACKSIDE = 0xFF00000000000000ull;
constexpr u64 KINGSIDE  = 0x9000000000000090ull;
constexpr u64 QUEENSIDE = 0x1100000000000011ull;
constexpr u64 SAFEPATH  = 0x6c0000000000006cull;
constexpr u64 PASSING   = 0x6e0000000000006eull;
constexpr u64 RIGHTS    = 0x6e0000000000006eull;

}; // namespace

#endif