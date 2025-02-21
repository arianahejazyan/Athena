#ifndef CHESS_H
#define CHESS_H

#include <cstdint>

namespace ELSA
{

using u64 = uint64_t;
using u32 = uint32_t;
using u16 = uint16_t;
using u8  = uint8_t;

enum Square : u8
{
    A1, B1, C1, D1, E1, F1, G1, H1,
    A2, B2, C2, D2, E2, F2, G2, H2,
    A3, B3, C3, D3, E3, F3, G3, H3,
    A4, B4, C4, D4, E4, F4, G4, H4,
    A5, B5, C5, D5, E5, F5, G5, H5,
    A6, B6, C6, D6, E6, F6, G6, H6,
    A7, B7, C7, D7, E7, F7, G7, H7,
    A8, B8, C8, D8, E8, F8, G8, H8, OFFBOARD, SQUARE_NB
};

enum PieceType : u8 
{ 
    NO_PIECE_TYPE,
    PAWN, 
    KNIGHT, 
    BISHOP, 
    ROOK, 
    QUEEN, 
    KING, 
    PIECE_TYPE_NB 
};

enum Piece : u8
{
    NO_PIECE,

    W_PAWN, W_KNIGHT, W_BISHOP, W_ROOK, W_QUEEN, W_KING,
    B_PAWN, B_KNIGHT, B_BISHOP, B_ROOK, B_QUEEN, B_KING,

    PIECE_NB
};

enum Player : u8
{ 
    WHITE, 
    BLACK, 

    PLAYER_NB 
};

enum CastlingRights : u8
{
    NO_CASTLING,

    WHITESIDE = 0b0011U,
    BLACKSIDE = 0b1100U,
    KINGSIDE  = 0b0101U,
    QUEENSIDE = 0b1010U,

    CASTLING_RIGHT_NB = 16U
};

enum File : u8
{
    FILE_A,
    FILE_B,
    FILE_C,
    FILE_D,
    FILE_E,
    FILE_F,
    FILE_G,
    FILE_H,
    FILE_NB
};

enum Rank : u8
{
    RANK_1,
    RANK_2,
    RANK_3,
    RANK_4,
    RANK_5,
    RANK_6,
    RANK_7,
    RANK_8,
    RANK_NB
};

enum MoveFlag : u16
{
    SIMPLE,
    ENPASS,
    CASTLE,
    EVOLVE,
};

PieceType getPieceType(Piece piece);

Player getPlayer(Piece piece);

}; // namespace

#endif