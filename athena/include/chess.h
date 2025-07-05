#ifndef CHESS_H
#define CHESS_H

#include <cstdint>
#include <array>
#include <vector>

namespace athena
{

/******************** ndrray ********************/

template<typename T, size_t... Dims>
struct ndarray_t;

template<typename T, size_t First, size_t... Rest>
struct ndarray_t<T, First, Rest...> { using type = std::array<typename ndarray_t<T, Rest...>::type, First>; };

template<typename T>
struct ndarray_t<T> { using type = T; };

template<typename T, size_t... Dims>
using ndarray = typename ndarray_t<T, Dims...>::type;

/******************** enum ********************/

enum Square : uint8_t 
{
    A1,  B1,  C1,  D1,  E1,  F1,  G1,  H1,  I1,  J1,  K1,  L1,  M1,  N1,  O1,  P1,
    A2,  B2,  C2,  D2,  E2,  F2,  G2,  H2,  I2,  J2,  K2,  L2,  M2,  N2,  O2,  P2,
    A3,  B3,  C3,  D3,  E3,  F3,  G3,  H3,  I3,  J3,  K3,  L3,  M3,  N3,  O3,  P3,
    A4,  B4,  C4,  D4,  E4,  F4,  G4,  H4,  I4,  J4,  K4,  L4,  M4,  N4,  O4,  P4,
    A5,  B5,  C5,  D5,  E5,  F5,  G5,  H5,  I5,  J5,  K5,  L5,  M5,  N5,  O5,  P5,
    A6,  B6,  C6,  D6,  E6,  F6,  G6,  H6,  I6,  J6,  K6,  L6,  M6,  N6,  O6,  P6,
    A7,  B7,  C7,  D7,  E7,  F7,  G7,  H7,  I7,  J7,  K7,  L7,  M7,  N7,  O7,  P7,
    A8,  B8,  C8,  D8,  E8,  F8,  G8,  H8,  I8,  J8,  K8,  L8,  M8,  N8,  O8,  P8,
    A9,  B9,  C9,  D9,  E9,  F9,  G9,  H9,  I9,  J9,  K9,  L9,  M9,  N9,  O9,  P9,
    A10, B10, C10, D10, E10, F10, G10, H10, I10, J10, K10, L10, M10, N10, O10, P10,
    A11, B11, C11, D11, E11, F11, G11, H11, I11, J11, K11, L11, M11, N11, O11, P11,
    A12, B12, C12, D12, E12, F12, G12, H12, I12, J12, K12, L12, M12, N12, O12, P12,
    A13, B13, C13, D13, E13, F13, G13, H13, I13, J13, K13, L13, M13, N13, O13, P13,
    A14, B14, C14, D14, E14, F14, G14, H14, I14, J14, K14, L14, M14, N14, O14, P14,
    A15, B15, C15, D15, E15, F15, G15, H15, I15, J15, K15, L15, M15, N15, O15, P15,
    A16, B16, C16, D16, E16, F16, G16, H16, I16, J16, K16, L16, M16, N16, O16, P16, OFFBOARD = 0ULL
};

enum Piece : uint8_t
{
    King   = 0,
    Knight = 1,
    Bishop = 2,
    Rook   = 3,
    Queen  = 4,
    Pawn   = 5,
    Empty  = 6,
    Stone  = 7,
};

enum Color : uint8_t
{
    Red    = 0,
    Blue   = 1,
    Yellow = 2,
    Green  = 3,
    None   = 4,
};

enum Guild : uint8_t
{
    RY = 0,
    BG = 1,
};

enum Offset : int
{
    F =  0,
    U = +1,
    D = -1,
    R = +1,
    L = -1,
};

enum Shift : int
{
    N  = +16,
    S  = -16,
    E  = +1 ,
    W  = -1 ,
};

enum Side : uint8_t
{
    KingSide  = 0,
    QueenSide = 1,
};

enum MoveFlag : uint8_t
{
    Noisy = 0,
    Quiet = 1,
};

enum MoveNature : uint8_t
{
    Jumper = 0,
    Slider = 1,
    Pushed = 2,
    Stride = 3,
    Strike = 4,
    Evolve = 5,
    Enpass = 6,
    Castle = 7,
};

enum GameSetup : uint8_t
{
    Classic = 0,
    Modern  = 1,
};

enum TimeControl : uint8_t
{
    Delay    = 0,
    Increase = 1,
};

/******************** class ********************/

class PieceClass
{
    private:

        uint8_t encoded;

    public:

        constexpr PieceClass(uint8_t encoded = 0) noexcept
            : encoded(encoded) {}

        constexpr PieceClass(Piece piece, Color color) noexcept 
            : encoded((color << 3) | piece) {}

        constexpr inline auto piece() const noexcept {
            return static_cast<Piece>(encoded & 0x7);
        }

        constexpr inline auto color() const noexcept {
            return static_cast<Color>(encoded >> 3);
        }

        constexpr operator uint32_t() const noexcept { return static_cast<uint32_t>(encoded); }
};

class Move
{
    private:

        uint32_t encoded;

    public:

        constexpr Move(uint32_t encoded = 0)
            : encoded(encoded) {}

        constexpr Move(Square source, Square target, MoveNature nature, MoveFlag flag)
            : encoded(source | (target << 8) | (nature << 16) | (flag  << 19)) {}

        constexpr Move(Square source, Square target, Side side)
            : encoded(source | (target << 8) | (Castle << 16) | (Quiet << 19) | (side  << 20)) {}    

        constexpr Move(Square source, Square target, Color color)
            : encoded(source | (target << 8) | (Enpass << 16) | (Noisy << 19) | (color << 21)) {}

        constexpr Move(Square source, Square target, PieceClass pc)
            : encoded(source | (target << 8) | (Evolve << 16) | (Noisy << 19) | (pc    << 24)) {}

        inline auto flag() const noexcept {
            return static_cast<MoveFlag>((encoded >> 19) & 0x1);
        }

        inline auto source() const noexcept { 
            return static_cast<Square>(encoded & 0xFF);
        }

        inline auto target() const noexcept {
            return static_cast<Square>((encoded >> 8) & 0xFF);
        }

        inline auto castle() const noexcept {
            return static_cast<Side>((encoded >> 20) & 0x1);
        }

        inline auto enpass() const noexcept {
            return static_cast<Color>((encoded >> 21) & 0x7);
        }

        inline auto evolve() const noexcept {
            return static_cast<PieceClass>((encoded >> 24) & 0x3F);
        }

        inline auto nature() const noexcept {
            return static_cast<MoveNature>((encoded >> 16) & 0x7);
        }
};

/******************** constant ********************/

constexpr PieceClass EMPTY(Empty, None);
constexpr PieceClass STONE(Stone, None);

constexpr std::size_t BOARDSIZE = 160;
constexpr std::size_t RANK_NB = 16;
constexpr std::size_t FILE_NB = 16;
constexpr std::size_t SQUARE_NB = RANK_NB * FILE_NB;

constexpr std::size_t PIECE_NB = 8;
constexpr std::size_t COLOR_NB = 5;
constexpr std::size_t PIECECLASS_NB = PIECE_NB * COLOR_NB;

constexpr std::size_t SIDE_NB = 2;
constexpr std::size_t OPPONENT_NB = 2;
constexpr std::size_t TEAMMATE_NB = 2;
constexpr std::size_t GAMESETUP_NB = 2;

constexpr int CHUNK_NB = 4;
constexpr int MAX_MOVES = 256;

/******************** method ********************/

constexpr inline auto chunkSQ(Square sq) noexcept { return sq >> 6; }
constexpr inline auto indexSQ(Square sq) noexcept { return sq & 63; }

constexpr inline auto rankSQ(Square sq)  noexcept { return sq >> 4;  }
constexpr inline auto fileSQ(Square sq)  noexcept { return sq & 0xF; }

constexpr inline auto makeSQ(int r, int f) noexcept { return static_cast<Square>((r << 4) | f); }

constexpr bool isValidSquare(int r, int f) noexcept
{
    bool b1 = (1 <= r  && r <= 14);
    bool b2 = (1 <= f  && f <= 14);
    bool b3 = (3 >= r  && f <= 3); 
    bool b4 = (3 >= r  && f >= 12); 
    bool b5 = (12 <= r && f <= 3); 
    bool b6 = (12 <= r && f >= 12); 

    return (b1 && b2) && !(b3 || b4 || b5 || b6);
}

inline auto next(Color color) noexcept {
    return static_cast<Color>((color + 1) & 0b11);
}

constexpr auto ALL_SQUARES = []
{
    std::array<Square, SQUARE_NB> arr{};
    for (int i = 0; i < SQUARE_NB; i++)
        arr[i] = Square(i);
    return arr;
}();

constexpr auto VALID_SQUARES = []
{
    std::array<Square, BOARDSIZE> arr{};
    int i = 0;
    for (Square sq: ALL_SQUARES)
    {
        auto r = rankSQ(sq);
        auto f = fileSQ(sq);
        if (isValidSquare(r,f)) arr[i++] = sq;
    }
    return arr;
}();

constexpr std::array<Side, SIDE_NB> SIDES = {KingSide, QueenSide};

constexpr std::array<Color, COLOR_NB - 1> COLORS = {Red, Blue, Yellow, Green};
constexpr std::array<Piece, PIECE_NB - 2> PIECES = {King, Knight, Bishop, Rook, Queen, Pawn};


// e.g. Red -> RY
inline auto toGuild(Color color) noexcept {
    return static_cast<Guild>(color & 0b1);
}

// eg Red -> Yellow
constexpr inline auto ally(Color color) noexcept {
    return color ^ 0b10;
}

inline Square operator+(Square sq, Shift shift) {
    return static_cast<Square>(static_cast<int>(sq) + static_cast<int>(shift));
}

inline Square operator-(Square sq, Shift shift) {
    return static_cast<Square>(static_cast<int>(sq) - static_cast<int>(shift));
}

constexpr ndarray<Move, GAMESETUP_NB, COLOR_NB - 1, SIDE_NB> CASTLE_MOVES =
{{{{
    { Move(I2,  K2 , KingSide), Move(I2,  G2 , QueenSide) },   
    { Move(B9,  B11, KingSide), Move(B9,  B7 , QueenSide) },  
    { Move(H15, F15, KingSide), Move(H15, J15, QueenSide) }, 
    { Move(O8,  O6 , KingSide), Move(O8,  O10, QueenSide) },
}},{{
    { Move(I2 , K2 , KingSide), Move(I2 , G2 , QueenSide) }, 
    { Move(B8 , B6 , KingSide), Move(B8 , B10, QueenSide) },
    { Move(H15, F15, KingSide), Move(H15, J15, QueenSide) },
    { Move(O9 , O11, KingSide), Move(O9 , O7 , QueenSide) },
}}}};

constexpr ndarray<Color, COLOR_NB - 1, OPPONENT_NB> OPPONENTS =
{{
    { Blue, Green  },
    { Red , Yellow },
    { Blue, Green  },
    { Red , Yellow },
}};

constexpr ndarray<Color, COLOR_NB - 1, TEAMMATE_NB> TEAMMATES =
{{
    { Red , Yellow },
    { Blue, Green  },
    { Red , Yellow },
    { Blue, Green  },
}};

constexpr ndarray<Shift, COLOR_NB - 1   > PUSH_DELTA = { N, E, S, W };
constexpr ndarray<Shift, COLOR_NB - 1, 2> TAKE_DELTA = 
{{
    { (Shift)(N + E), (Shift)(N + W) },
    { (Shift)(S + E), (Shift)(N + E) },
    { (Shift)(S + W), (Shift)(S + E) },
    { (Shift)(N + W), (Shift)(S + W) }, 
}};

} // namespace athena

#endif // #ifndef CHESS_H