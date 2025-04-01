#ifndef CHESS_H
#define CHESS_H

#include <cassert>
#include "types.h"

namespace Talia
{

enum Square : u8
{
    A1, B1, C1, D1, E1, F1, G1, H1,
    A2, B2, C2, D2, E2, F2, G2, H2,
    A3, B3, C3, D3, E3, F3, G3, H3,
    A4, B4, C4, D4, E4, F4, G4, H4,
    A5, B5, C5, D5, E5, F5, G5, H5,
    A6, B6, C6, D6, E6, F6, G6, H6,
    A7, B7, C7, D7, E7, F7, G7, H7,
    A8, B8, C8, D8, E8, F8, G8, H8, OFFBOARD, SQUARE_NB = 64u
};

enum Piece : u8 
{ 
    NO_PIECE = 6,

    PAWN   = 0, 
    KNIGHT = 1, 
    BISHOP = 2, 
    ROOK   = 3, 
    QUEEN  = 4, 
    KING   = 5, 
    
    PIECE_NB = 6 
};

enum Color : u8
{ 
    NO_COLOR = 2,

    WHITE = 0, 
    BLACK = 1, 

    COLOR_NB = 2
};

enum PieceType : u8
{
    NO_PIECE_TYPE = 12,

    W_PAWN   = (2 * PAWN + WHITE),  W_KNIGHT = (2 * KNIGHT + WHITE),  W_BISHOP = (2 * BISHOP + WHITE),
    W_ROOK   = (2 * ROOK + WHITE),  W_QUEEN  = (2 * QUEEN  + WHITE),  W_KING   = (2 * KING   + WHITE),
    B_PAWN   = (2 * PAWN + BLACK),  B_KNIGHT = (2 * KNIGHT + BLACK),  B_BISHOP = (2 * BISHOP + BLACK),
    B_ROOK   = (2 * ROOK + BLACK),  B_QUEEN  = (2 * QUEEN  + BLACK),  B_KING   = (2 * KING   + BLACK),

    PIECE_TYPE_NB = 12
};

enum File : u64
{
    FILEA = 0x0101010101010101ull,
    FILEB = 0x0202020202020202ull,
    FILEC = 0x0404040404040404ull,
    FILED = 0x0808080808080808ull,
    FILEE = 0x1010101010101010ull,
    FILEF = 0x2020202020202020ull,
    FILEG = 0x4040404040404040ull,
    FILEH = 0x8080808080808080ull,    

    FILE_NB = 8
};

enum Rank : u64
{
    RANK1 = 0x00000000000000FFull,
    RANK2 = 0x000000000000FF00ull,
    RANK3 = 0x0000000000FF0000ull,
    RANK4 = 0x00000000FF000000ull,
    RANK5 = 0x000000FF00000000ull,
    RANK6 = 0x0000FF0000000000ull,
    RANK7 = 0x00FF000000000000ull,
    RANK8 = 0xFF00000000000000ull,

    RANK_NB = 8
};

enum Castle : u64
{
    NO_CASTLE = 0ull,

    RIGHT = (FILEA | FILEE | FILEH) & (RANK1 | RANK8),

    WHITESIDE = RANK1,
    BLACKSIDE = RANK8,

    KINGSIDE  = (FILEE | FILEF | FILEG | FILEH),
    QUEENSIDE = (FILEA | FILEB | FILEC | FILED | FILEE),

    SAFE = (FILEC | FILED | FILEE | FILEF | FILEG),
    PASS = (FILEB | FILEC | FILED | FILEF | FILEG),

    CASTLING_NB = 64ull
};

enum Offset : int
{
    NORTH = +8,
    EAST  = +1,
    SOUTH = -8,
    WEST  = -1,

    NE = NORTH + EAST,
    NW = NORTH + WEST,
    SE = SOUTH + EAST,
    SW = SOUTH + WEST,

    NN = NORTH + NORTH,
    SS = SOUTH + SOUTH,

    EE = EAST + EAST,
    WW = WEST + WEST,

    NNE = NN + EAST,
    NNW = NN + WEST,
    SSE = SS + EAST,
    SSW = SS + WEST,

    NEE = EE + NORTH,
    NWW = WW + NORTH,
    SEE = EE + SOUTH,
    SWW = WW + SOUTH,

    OFFSET_NB = 8,
};

enum MoveFlag : u16
{
    SIMPLE,
    ENPASS,
    CASTLE,
    DOUBLE,

    FLAG_KNIGHT, 
    FLAG_BISHOP, 
    FLAG_ROOK, 
    FLAG_QUEEN, 
};

enum MoveType : u8
{
    QUIET,
    NOISY,
    LEGAL,
};

enum GamePhase: uint8_t 
{
    MIDGAME = 0,
    ENDGAME = 1,

    GAMEPHASE_NB = 2
};

[[nodiscard]] constexpr inline Piece toPiece(const PieceType type)
{
    assert(W_PAWN <= type && type <= B_KING);

    return static_cast<Piece>(type >> 1);
}

[[nodiscard]] constexpr inline Color toColor(const PieceType type)
{
    assert(W_PAWN <= type && type <= B_KING);

    return static_cast<Color>(type & 1);
}

[[nodiscard]] constexpr inline PieceType toPieceType(const Piece piece, const Color color)
{
    assert(PAWN  <= piece && piece <= KING);
    assert(WHITE <= color && color <= BLACK);

    return static_cast<PieceType>(piece + (color << 0x3u));
}


[[nodiscard]] inline constexpr Castle operator&(const Castle lhs, const Castle rhs)
{
    return static_cast<Castle>(static_cast<u64>(lhs) & static_cast<u64>(rhs));
}

[[nodiscard]] inline constexpr bool operator==(const Castle lhs, const Castle rhs)
{
    return static_cast<u64>(lhs) == static_cast<u64>(rhs);
}

inline constexpr Square& operator++(Square& sq)
{
    return sq = static_cast<Square>(static_cast<u8>(sq) + 1);
}

inline constexpr Square operator++(Square& sq, int)
{
    Square tmp = sq;
    ++sq;
    return tmp;
}

[[nodiscard]] inline Color next(const Color color)
{
    assert(WHITE <= color && color <= BLACK);

    return static_cast<Color>(color ^ 1);
}

inline constexpr PieceType& operator++(PieceType& pt)
{
    return pt = static_cast<PieceType>(static_cast<u8>(pt) + 1);
}

inline constexpr PieceType operator++(PieceType& pt, int)
{
    PieceType tmp = pt;
    ++pt;
    return tmp;
}

constexpr BitBoard EMPTY_BOARD = 0ULL;
constexpr size_t   MAX_MOVES = 256;

} // namespace

#endif