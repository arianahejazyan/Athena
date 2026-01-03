#pragma once

#include <array>
#include <cstdint>
#include <string_view>

namespace athena
{

enum class Square : uint8_t 
{
    A1 , B1 , C1 , D1 , E1 , F1 , G1 , H1 , I1 , J1 , K1 , L1 , M1 , N1 , O1 , P1 ,
    A2 , B2 , C2 , D2 , E2 , F2 , G2 , H2 , I2 , J2 , K2 , L2 , M2 , N2 , O2 , P2 ,
    A3 , B3 , C3 , D3 , E3 , F3 , G3 , H3 , I3 , J3 , K3 , L3 , M3 , N3 , O3 , P3 ,
    A4 , B4 , C4 , D4 , E4 , F4 , G4 , H4 , I4 , J4 , K4 , L4 , M4 , N4 , O4 , P4 ,
    A5 , B5 , C5 , D5 , E5 , F5 , G5 , H5 , I5 , J5 , K5 , L5 , M5 , N5 , O5 , P5 ,
    A6 , B6 , C6 , D6 , E6 , F6 , G6 , H6 , I6 , J6 , K6 , L6 , M6 , N6 , O6 , P6 ,
    A7 , B7 , C7 , D7 , E7 , F7 , G7 , H7 , I7 , J7 , K7 , L7 , M7 , N7 , O7 , P7 ,
    A8 , B8 , C8 , D8 , E8 , F8 , G8 , H8 , I8 , J8 , K8 , L8 , M8 , N8 , O8 , P8 ,
    A9 , B9 , C9 , D9 , E9 , F9 , G9 , H9 , I9 , J9 , K9 , L9 , M9 , N9 , O9 , P9 ,
    A10, B10, C10, D10, E10, F10, G10, H10, I10, J10, K10, L10, M10, N10, O10, P10,
    A11, B11, C11, D11, E11, F11, G11, H11, I11, J11, K11, L11, M11, N11, O11, P11,
    A12, B12, C12, D12, E12, F12, G12, H12, I12, J12, K12, L12, M12, N12, O12, P12,
    A13, B13, C13, D13, E13, F13, G13, H13, I13, J13, K13, L13, M13, N13, O13, P13,
    A14, B14, C14, D14, E14, F14, G14, H14, I14, J14, K14, L14, M14, N14, O14, P14,
    A15, B15, C15, D15, E15, F15, G15, H15, I15, J15, K15, L15, M15, N15, O15, P15,
    A16, B16, C16, D16, E16, F16, G16, H16, I16, J16, K16, L16, M16, N16, O16, P16, Offboard = 0
};

enum class Piece : uint8_t
{
    Pawn   = 0,
    Knight = 1,
    Bishop = 2,
    Rook   = 3,
    Queen  = 4,
    King   = 5,
    Stone  = 6,
    Empty  = 7,
};

enum class Color : uint8_t
{
    Red    = 0,
    Blue   = 1,
    Yellow = 2,
    Green  = 3,
    None   = 4,
};

enum class Side : uint8_t
{
    KingSide  = 0,
    QueenSide = 1,
};

enum class Castle : uint8_t
{
    rKS = 1 << ((static_cast<uint8_t>(Color:: Red   ) * 2) + static_cast<uint8_t>(Side::KingSide )), // 0b00000001
    rQS = 1 << ((static_cast<uint8_t>(Color:: Red   ) * 2) + static_cast<uint8_t>(Side::QueenSide)),
    bKS = 1 << ((static_cast<uint8_t>(Color:: Blue  ) * 2) + static_cast<uint8_t>(Side::KingSide )),
    bQS = 1 << ((static_cast<uint8_t>(Color:: Blue  ) * 2) + static_cast<uint8_t>(Side::QueenSide)),
    yKS = 1 << ((static_cast<uint8_t>(Color:: Yellow) * 2) + static_cast<uint8_t>(Side::KingSide )),
    yQS = 1 << ((static_cast<uint8_t>(Color:: Yellow) * 2) + static_cast<uint8_t>(Side::QueenSide)),
    gKS = 1 << ((static_cast<uint8_t>(Color:: Green ) * 2) + static_cast<uint8_t>(Side::KingSide )),
    gQS = 1 << ((static_cast<uint8_t>(Color:: Green ) * 2) + static_cast<uint8_t>(Side::QueenSide)),
};

enum class MoveType : uint8_t
{
    Simple = 0,
    Stride = 1,
    Enpass = 2,
    Castle = 3,
    Evolve = 4,
};

enum class MoveFlag : uint8_t
{
    Noisy = 0,
    Quiet = 1,
};

constexpr std::size_t 
RANK_NB        = 16,
FILE_NB        = 16,
SQUARE_NB      = 256,
PIECE_NB       = 6,
COLOR_NB       = 4,
PIECECLASS_NB  = 64,
SIDE_NB        = 2,
CASTLE_NB      = 8,
MOVETYPE_NB    = 5,
MOVEFLAG_NB    = 2,
MOVE_NB        = 256,
PLAY_NB        = 32,
HOME_RANK      = 2,
PROMOTION_RANK = 11,
CRAWL_NB       = 8,
SLIDE_NB       = 4,
PUSH_NB        = 2,
TAKE_NB        = 2,
CHUNK_NB       = 4,
CHUNK_SIZE     = 64,
CACHELINE_SIZE = 64;

using FEN = std::string_view;
static constexpr FEN STARTPOS =
#ifdef GAME_SETUP_MODERN
"R-0,0,0,0-1,1,1,1-1,1,1,1-0,0,0,0-0-"
"x,x,x,yR,yN,yB,yK,yQ,yB,yN,yR,x,x,x/"
"x,x,x,yP,yP,yP,yP,yP,yP,yP,yP,x,x,x/"
"x,x,x,8,x,x,x/"
"bR,bP,10,gP,gR/"
"bN,bP,10,gP,gN/"
"bB,bP,10,gP,gB/"
"bQ,bP,10,gP,gK/"
"bK,bP,10,gP,gQ/"
"bB,bP,10,gP,gB/"
"bN,bP,10,gP,gN/"
"bR,bP,10,gP,gR/"
"x,x,x,8,x,x,x/"
"x,x,x,rP,rP,rP,rP,rP,rP,rP,rP,x,x,x/"
"x,x,x,rR,rN,rB,rQ,rK,rB,rN,rR,x,x,x";    
#else
"R-0,0,0,0-1,1,1,1-1,1,1,1-0,0,0,0-0-"
"x,x,x,yR,yN,yB,yK,yQ,yB,yN,yR,x,x,x/"
"x,x,x,yP,yP,yP,yP,yP,yP,yP,yP,x,x,x/"
"x,x,x,8,x,x,x/"
"bR,bP,10,gP,gR/"
"bN,bP,10,gP,gN/"
"bB,bP,10,gP,gB/"
"bK,bP,10,gP,gQ/"
"bQ,bP,10,gP,gK/"
"bB,bP,10,gP,gB/"
"bN,bP,10,gP,gN/"
"bR,bP,10,gP,gR/"
"x,x,x,8,x,x,x/"
"x,x,x,rP,rP,rP,rP,rP,rP,rP,rP,x,x,x/"
"x,x,x,rR,rN,rB,rQ,rK,rB,rN,rR,x,x,x";
#endif

class PieceClass
{
    public:
    PieceClass() = default;
    constexpr PieceClass(Color color, Piece piece) noexcept : encoded_((static_cast<uint8_t>(color) << 3) | static_cast<uint8_t>(piece)) {}

    Piece piece() const noexcept { return Piece(encoded_ &  7); }
    Color color() const noexcept { return Color(encoded_ >> 3); }

    bool operator==(const PieceClass& other) const noexcept { return encoded_ == other.encoded_; }
    bool operator!=(const PieceClass& other) const noexcept { return encoded_ != other.encoded_; }
    
    private:
    uint8_t encoded_;

    public:
    static consteval PieceClass Stone() { return PieceClass(Color::None, Piece::Stone); };
    static consteval PieceClass Empty() { return PieceClass(Color::None, Piece::Empty); };
};

inline Square buildSquare(int rank, int file) { 
    return Square(rank * FILE_NB + file);
}

enum class Offset : int
{
    N = +16,
    S = -16,
    E = +1 ,
    W = -1 ,
    Z =  0 ,
};

consteval Offset operator+(Offset lhs, Offset rhs) noexcept {
    return Offset(static_cast<int>(lhs) + static_cast<int>(rhs));
}

consteval std::array<Offset, CRAWL_NB> crawl_offsets(Piece piece)
{
    std::array<Offset, CRAWL_NB> KNIGHT_OFFSETS =
    {{
        Offset::S + Offset::S + Offset::W,
        Offset::S + Offset::S + Offset::E,
        Offset::S + Offset::W + Offset::W,
        Offset::S + Offset::E + Offset::E,
        Offset::N + Offset::W + Offset::W,
        Offset::N + Offset::E + Offset::E,
        Offset::N + Offset::N + Offset::W,
        Offset::N + Offset::N + Offset::E,
    }};

    std::array<Offset, CRAWL_NB> KING_OFFSETS = 
    {{
        Offset::S + Offset::W,
        Offset::S            ,
        Offset::S + Offset::E,
        Offset::W            ,
        Offset::E            ,
        Offset::N + Offset::W,
        Offset::N            ,
        Offset::N + Offset::E,
    }};

    return piece == Piece::Knight ? KNIGHT_OFFSETS : KING_OFFSETS;
}

// consteval std::array<uint8_t, CRAWL_NB> crawl_offsets(Piece piece)
// {
//     std::array<uint8_t, CRAWL_NB> KNIGHT_OFFSETS =
//     {{
//         Offset(-2, -1),
//         Offset(-2, +1),
//         Offset(-1, -2),
//         Offset(-1, +2),
//         Offset(+1, -2),
//         Offset(+1, +2),
//         Offset(+2, -1),
//         Offset(+2, +1),
//     }};

//     std::array<uint8_t, CRAWL_NB> KING_OFFSETS = 
//     {{
//         Offset(-1, -1),
//         Offset(-1, 0 ),
//         Offset(-1, +1),
//         Offset(0 , -1),
//         Offset(0 , +1),
//         Offset(+1, -1),
//         Offset(+1, 0 ),
//         Offset(+1, +1),
//     }};

//     return piece == Piece::Knight ? KNIGHT_OFFSETS : KING_OFFSETS;
// }

consteval std::array<Offset, SLIDE_NB> slide_offsets(Piece piece)
{
    std::array<Offset, SLIDE_NB> BISHOP_OFFSETS =
    {
        Offset::S + Offset::W,
        Offset::S + Offset::E,
        Offset::N + Offset::W,
        Offset::N + Offset::E,
    };

    std::array<Offset, SLIDE_NB> ROOK_OFFSETS = 
    {
        Offset::S,
        Offset::W,
        Offset::E,
        Offset::N,
    };

    return piece == Piece::Bishop ? BISHOP_OFFSETS : ROOK_OFFSETS;
}

// consteval std::array<uint8_t, SLIDE_NB> slide_offsets(Piece piece)
// {
//     std::array<uint8_t, SLIDE_NB> BISHOP_OFFSETS =
//     {{
//         Offset(-1, -1),
//         Offset(-1, +1),
//         Offset(+1, -1),
//         Offset(+1, +1),
//     }};

//     std::array<uint8_t, SLIDE_NB> ROOK_OFFSETS = 
//     {{
//         Offset(-1, 0 ),
//         Offset(0 , -1),
//         Offset(0 , +1),
//         Offset(+1, 0 ),
//     }};

//     return piece == Piece::Bishop ? BISHOP_OFFSETS : ROOK_OFFSETS;
// }

consteval std::array<Offset, PUSH_NB> push_offsets(Color color)
{
    std::array<std::array<Offset, PUSH_NB>, COLOR_NB> PUSH_OFFSETS =
    {{
        {{ Offset::N, Offset::N + Offset::N }},
        {{ Offset::E, Offset::E + Offset::E }},
        {{ Offset::S, Offset::S + Offset::S }},
        {{ Offset::W, Offset::W + Offset::W }},
    }};

    return PUSH_OFFSETS[static_cast<uint8_t>(color)];
}

consteval std::array<Offset, TAKE_NB> take_offsets(Color color)
{
    std::array<std::array<Offset, TAKE_NB>, COLOR_NB> TAKE_OFFSETS =
    {{
        {{ Offset::N + Offset::W, Offset::N + Offset::E }},
        {{ Offset::E + Offset::S, Offset::E + Offset::N }},
        {{ Offset::S + Offset::W, Offset::S + Offset::E }},
        {{ Offset::W + Offset::S, Offset::W + Offset::N }},
    }};

    return TAKE_OFFSETS[static_cast<uint8_t>(color)];
}

// consteval std::array<uint8_t, PUSH_NB> push_offsets(Color color)
// {
//     std::array<std::array<uint8_t, PUSH_NB>, COLOR_NB> PUSH_OFFSETS =
//     {{
//         {{ Offset(+1, 0 ), Offset(+2, 0 ) }},
//         {{ Offset(0 , +1), Offset(0 , +2) }},
//         {{ Offset(-1, 0 ), Offset(-2, 0 ) }},
//         {{ Offset(0 , -1), Offset(0 , -2) }},
//     }};

//     return PUSH_OFFSETS[static_cast<uint8_t>(color)];
// }

// consteval std::array<uint8_t, TAKE_NB> take_offsets(Color color)
// {
//     std::array<std::array<uint8_t, TAKE_NB>, COLOR_NB> TAKE_OFFSETS =
//     {{
//         {{ Offset(+1, -1), Offset(+1, +1) }},
//         {{ Offset(-1, +1), Offset(+1, +1) }},
//         {{ Offset(-1, -1), Offset(-1, +1) }},
//         {{ Offset(-1, -1), Offset(+1, -1) }},
//     }};

//     return TAKE_OFFSETS[static_cast<uint8_t>(color)];
// }

inline constexpr uint8_t index(Square square) noexcept { return static_cast<uint8_t>(square) % CHUNK_SIZE; }
inline constexpr uint8_t chunk(Square square) noexcept { return static_cast<uint8_t>(square) / CHUNK_SIZE; }

inline constexpr int rank(Square square) noexcept { return static_cast<uint8_t>(square) / FILE_NB; }
inline constexpr int file(Square square) noexcept { return static_cast<uint8_t>(square) % FILE_NB; }

inline Color next(Color color) noexcept { return Color((static_cast<uint8_t>(color) + 1) & 0b011); }
inline Color ally(Color color) noexcept { return Color((static_cast<uint8_t>(color) + 2) & 0b011); }
inline Color prev(Color color) noexcept { return Color((static_cast<uint8_t>(color) + 3) & 0b011); }

// inline bool isTeammate(Color c1, Color c2) noexcept { return (static_cast<uint8_t>(c1) ^ static_cast<uint8_t>(c2)) == 0b010; }
// inline bool isOpponent(Color c1, Color c2) noexcept { return (static_cast<uint8_t>(c1) ^ static_cast<uint8_t>(c2)) != 0b010; }

inline bool isTeammate(Color c1, Color c2) noexcept { return ((static_cast<uint8_t>(c1) ^ static_cast<uint8_t>(c2)) & 0b1) == 0b0; }
inline bool isOpponent(Color c1, Color c2) noexcept { return ((static_cast<uint8_t>(c1) ^ static_cast<uint8_t>(c2)) & 0b1) == 0b1; }

template<Color color>
bool isHomerank(Square square) noexcept
{
    auto r = rank(square);
    auto f = file(square);

    return 
    color == Color::Red    ? r == (          HOME_RANK) || r == (          HOME_RANK - 1) :
    color == Color::Blue   ? f == (          HOME_RANK) || f == (          HOME_RANK - 1) :
    color == Color::Yellow ? r == (RANK_NB - HOME_RANK) || r == (RANK_NB - HOME_RANK - 1) :
    color == Color::Green  ? f == (FILE_NB - HOME_RANK) || f == (FILE_NB - HOME_RANK - 1) : false;
}

template<Color color>
bool isPromotion(Square square) noexcept
{
    auto r = rank(square);
    auto f = file(square);

    return 
    color == Color::Red    ? r == (          PROMOTION_RANK    ) :
    color == Color::Blue   ? f == (          PROMOTION_RANK    ) : 
    color == Color::Yellow ? r == (RANK_NB - PROMOTION_RANK - 1) :
    color == Color::Green  ? f == (FILE_NB - PROMOTION_RANK - 1) : false;
}

inline constexpr bool isStone(Square square) noexcept
{
    auto r = rank(square);
    auto f = file(square);

    return (r == 0  || f == 0 ) ||
           (r == 15 || f == 15) ||
           (r <= 3  && f <= 3 ) ||
           (r >= 12 && f >= 12) ||
           (r <= 3  && f >= 12) ||
           (r >= 12 && f <= 3 );
}

inline constexpr Offset operator*(Offset lhs, int rhs) noexcept {
    return Offset(static_cast<int>(lhs) * rhs);
}

inline constexpr Square& operator+=(Square& square, uint8_t offset) noexcept {
    square = static_cast<Square>(static_cast<uint8_t>(square) + offset);
    return square;
}

inline constexpr  Square& operator+=(Square& square, Offset offset) noexcept {
    square = static_cast<Square>(static_cast<uint8_t>(square) + static_cast<int>(offset));
    return square;
}

inline constexpr Square operator++(Square& square, int) noexcept {
    Square old = square;
    square = static_cast<Square>(static_cast<uint8_t>(square) + 1);
    return old;
}

inline constexpr bool operator<=(Square lhs, Square rhs) noexcept {
    return static_cast<uint8_t>(lhs) <= static_cast<uint8_t>(rhs);
}

inline constexpr Square operator+(Square sq, uint8_t offset) noexcept {
    return static_cast<Square>(
        static_cast<uint8_t>(sq) + offset
    );
}

inline constexpr Square operator+(Square sq, Offset offset) noexcept {
    return static_cast<Square>(
        static_cast<uint8_t>(sq) + static_cast<int>(offset)
    );
}


inline Square operator+(Square lhs, Square rhs) noexcept {
    return static_cast<Square>(
        static_cast<uint8_t>(lhs) + static_cast<uint8_t>(rhs)
    );
}

inline Square operator/(Square lhs, uint8_t rhs) noexcept {
    return static_cast<Square>(
        static_cast<uint8_t>(lhs) / rhs
    );
}

inline Square operator>>(Square lhs, uint8_t rhs) noexcept {
    return static_cast<Square>(
        static_cast<uint8_t>(lhs) >> rhs
    );
}

consteval Castle buildCastle(Color color, Side side) noexcept {
    return static_cast<Castle>(1 << ((2 * static_cast<uint8_t>(color)) + static_cast<uint8_t>(side)));
}

template<Color color, Side side>
bool hasCastle(Castle castle) noexcept {
    return (static_cast<uint8_t>(castle) & static_cast<uint8_t>(buildCastle(color, side))) != 0;
}

constexpr Castle& operator&=(Castle& lhs, Castle rhs) noexcept {
    lhs = static_cast<Castle>(
        static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs)
    );
    return lhs;
}

inline std::array<Color, 2> opponents(Color color)
{
    return 
    color == Color::Red    ? std::array<Color, 2>({Color::Blue, Color::Green }) :
    color == Color::Blue   ? std::array<Color, 2>({Color::Red , Color::Yellow}) :
    color == Color::Yellow ? std::array<Color, 2>({Color::Blue, Color::Green }) :
    color == Color::Green  ? std::array<Color, 2>({Color::Red , Color::Yellow}) : std::array<Color, 2>({Color::None, Color::None});
}

inline std::array<Color, 2> teammates(Color color)
{
    return 
    color == Color::Red    ? std::array<Color, 2>({Color::Red , Color::Yellow}) :
    color == Color::Blue   ? std::array<Color, 2>({Color::Blue, Color::Green }) :
    color == Color::Yellow ? std::array<Color, 2>({Color::Red , Color::Yellow}) :
    color == Color::Green  ? std::array<Color, 2>({Color::Blue, Color::Green }) : std::array<Color, 2>({Color::None, Color::None});
}

consteval int max_step(Piece piece) noexcept
{
    return piece == Piece::Rook ? 13 : 10;
}

// Forward declaration
class Position;

class Move
{
    public:

    Move(Square source, Square target, MoveType type)
        : encoded_(static_cast<uint32_t>(source) | (static_cast<uint32_t>(target) << 24) | (static_cast<uint32_t>(type) << 8)) {}

    consteval
    Move(Square source, Square target, Side side)
        : encoded_(static_cast<uint32_t>(source) | (static_cast<uint32_t>(target) << 24) | (static_cast<uint32_t>(MoveType::Castle) << 8) | (static_cast<uint32_t>(side) << 18)) {}

    Move(Square source, Square target, Piece piece)
        : encoded_(static_cast<uint32_t>(source) | (static_cast<uint32_t>(target) << 24) | (static_cast<uint32_t>(MoveType::Evolve) << 8) | (static_cast<uint32_t>(piece) << 12)) {}

    Move(Square source, Square target, Color enpass)
        : encoded_(static_cast<uint32_t>(source) | (static_cast<uint32_t>(target) << 24) | (static_cast<uint32_t>(MoveType::Enpass) << 8) | (static_cast<uint32_t>(enpass) << 15)) {}

    Move() = default;

    Square source() const noexcept { 
        return Square(encoded_ & 0xFF); 
    }

    Square target() const noexcept { 
        return Square(encoded_ >> 24); 
    }

    Side castle() const noexcept { 
        return Side((encoded_ >> 18) & 0x1);
    }

    Color enpass() const noexcept { 
        return Color((encoded_ >> 15) & 0x7); 
    }

    Piece evolve() const noexcept { 
        return Piece((encoded_ >> 12) & 0x7);
    }

    MoveType type() const noexcept { 
        return MoveType((encoded_ >> 8) & 0x7); 
    }

    MoveFlag flag() const noexcept { 
        return MoveFlag((encoded_ >> 11) & 0x1); 
    }

    private:
    uint32_t encoded_;
};

consteval std::array<Square, 4> castle_squares(Color color, Side side)
{
    #ifdef GAME_SETUP_MODERN
    std::array<std::array<Square, 4>, COLOR_NB> KINGSIDE_SQUARES = 
    {{
        {Square::I2 , Square::K2 , Square::L2 , Square::J2 },
        {Square::B8 , Square::B6 , Square::B5 , Square::B7 }, 
        {Square::H15, Square::F15, Square::E15, Square::G15},  
        {Square::O9 , Square::O11, Square::O12, Square::O10}, 
    }};

    std::array<std::array<Square, 4>, COLOR_NB> QUEENSIDE_SQUARES = 
    {{
        {Square::I2 , Square::G2 , Square::E2 , Square::H2 },
        {Square::B8 , Square::B10, Square::B12, Square::B9 },
        {Square::H15, Square::J15, Square::L15, Square::I15},
        {Square::O9 , Square::O7 , Square::O5 , Square::O8 },
    }};
    #else
    std::array<std::array<Square, 4>, COLOR_NB> KINGSIDE_SQUARES = 
    {{
        {Square::I2 , Square::K2 , Square::L2 , Square::J2 },
        {Square::B9 , Square::B11, Square::B12, Square::B10},
        {Square::H15, Square::F15, Square::E15, Square::G15}, 
        {Square::O8 , Square::O6 , Square::O5 , Square::O7 }, 
    }};

    std::array<std::array<Square, 4>, COLOR_NB> QUEENSIDE_SQUARES = 
    {{
        {Square::I2 , Square::G2 , Square::E2 , Square::H2 },
        {Square::B9 , Square::B7 , Square::B5 , Square::B8 },
        {Square::H15, Square::J15, Square::L15, Square::I15},
        {Square::O8 , Square::O10, Square::O12, Square::O9 },
    }};
    #endif

    return 
    side == Side::KingSide 
    ?KINGSIDE_SQUARES[static_cast<uint8_t>(color)]: 
    QUEENSIDE_SQUARES[static_cast<uint8_t>(color)];
}

consteval std::array<Square, 3> path_squares(Color color, Side side)
{
    #ifdef GAME_SETUP_MODERN
    std::array<std::array<Square, 3>, COLOR_NB> KINGSIDE_SQUARES = 
    {{
        {Square::J2 , Square::K2 , Square::K2 },
        {Square::B6 , Square::B7 , Square::B7 }, 
        {Square::F15, Square::G15, Square::G15},  
        {Square::O10, Square::O11, Square::O11}, 
    }};

    std::array<std::array<Square, 3>, COLOR_NB> QUEENSIDE_SQUARES = 
    {{
        {Square::F2 , Square::G2 , Square::H2 },
        {Square::B9 , Square::B10, Square::B11},
        {Square::I15, Square::J15, Square::K15},
        {Square::O6 , Square::O7 , Square::O8 },
    }};
    #else
    std::array<std::array<Square, 3>, COLOR_NB> KINGSIDE_SQUARES = 
    {{
        {Square::J2 , Square::K2 , Square::K2 },
        {Square::B10, Square::B11, Square::B11},
        {Square::F15, Square::G15, Square::G15}, 
        {Square::O6 , Square::O7 , Square::O7 }, 
    }};

    std::array<std::array<Square, 3>, COLOR_NB> QUEENSIDE_SQUARES = 
    {{
        {Square::F2 , Square::G2 , Square::H2 },
        {Square::B6 , Square::B7 , Square::B8 },
        {Square::I15, Square::J15, Square::K15},
        {Square::O9 , Square::O10, Square::O11},
    }};
    #endif

    return 
    side == Side::KingSide 
    ?KINGSIDE_SQUARES[static_cast<uint8_t>(color)]: 
    QUEENSIDE_SQUARES[static_cast<uint8_t>(color)];
}

consteval std::array<Square, 2> safe_squares(Color color, Side side)
{
    #ifdef GAME_SETUP_MODERN
    std::array<std::array<Square, 2>, COLOR_NB> KINGSIDE_SQUARES = 
    {{
        {Square::I2 , Square::J2 },
        {Square::B7 , Square::B8 }, 
        {Square::G15, Square::H15}, 
        {Square::O9 , Square::O10}, 
    }};

    std::array<std::array<Square, 2>, COLOR_NB> QUEENSIDE_SQUARES = 
    {{
        {Square::H2 , Square::I2 },
        {Square::B8 , Square::B9 },
        {Square::H15, Square::I15},
        {Square::O8 , Square::O9 },
    }};
    #else
    std::array<std::array<Square, 2>, COLOR_NB> KINGSIDE_SQUARES = 
    {{
        {Square::I2 , Square::J2 },
        {Square::B9 , Square::B10},
        {Square::G15, Square::H15},
        {Square::O7 , Square::O8 }, 
    }};

    std::array<std::array<Square, 2>, COLOR_NB> QUEENSIDE_SQUARES = 
    {{
        {Square::H2 , Square::I2 },
        {Square::B8 , Square::B9 },
        {Square::H15, Square::I15},
        {Square::O8 , Square::O9 },
    }};
    #endif

    return 
    side == Side::KingSide
    ?KINGSIDE_SQUARES[static_cast<uint8_t>(color)]: 
    QUEENSIDE_SQUARES[static_cast<uint8_t>(color)];
}

inline Castle castling_rights(Square square) noexcept
{
    static constexpr std::array<Castle, SQUARE_NB> CASTLING_RIGHTS = []() consteval
    {
        std::array<Castle, SQUARE_NB> arr {};
        arr.fill(Castle(0xFF));

        for (Color color : {Color::Red, Color::Blue, Color::Yellow, Color::Green})
        {
            for (Side side : {Side::KingSide, Side::QueenSide})
            {
                auto king_source = castle_squares(color, side)[0];
                auto rook_source = castle_squares(color, side)[2];

                arr[static_cast<uint8_t>(king_source)] &= static_cast<Castle>(~static_cast<uint8_t>(buildCastle(color, side)));
                arr[static_cast<uint8_t>(rook_source)] &= static_cast<Castle>(~static_cast<uint8_t>(buildCastle(color, side)));
            }
        }

        return arr;
    }();

    return CASTLING_RIGHTS[static_cast<uint8_t>(square)];
}

// struct CastleSquares
// {
//     Square king_source;
//     Square king_target;
//     Square rook_source;
//     Square rook_target;
//     std::array<Square, 3> path;
//     std::array<Square, 2> safe;
// };

// inline constexpr std::array<std::array<std::array<CastleSquares, SIDE_NB>, COLOR_NB>, SETUP_NB> CASTLE_SQUARES = 
// {{
// {{
//     {{
//         {Square::I2 , Square::K2 , Square::L2 , Square::J2 , {Square::J2 , Square::K2 , Square::K2 }, {Square::I2 , Square::J2 }}, 
//         {Square::I2 , Square::G2 , Square::E2 , Square::H2 , {Square::F2 , Square::G2 , Square::H2 }, {Square::H2 , Square::I2 }},
//         }}, 
//     {{
//         {Square::B8 , Square::B6 , Square::B5 , Square::B7 , {Square::B6 , Square::B7 , Square::B7 }, {Square::B7 , Square::B8 }}, 
//         {Square::B8 , Square::B10, Square::B12, Square::B9 , {Square::B9 , Square::B10, Square::B11}, {Square::B8 , Square::B9 }},
//         }}, 
//     {{
//         {Square::H15, Square::F15, Square::E15, Square::G15, {Square::F15, Square::G15, Square::G15}, {Square::G15, Square::H15}}, 
//         {Square::H15, Square::J15, Square::L15, Square::I15, {Square::I15, Square::J15, Square::K15}, {Square::H15, Square::I15}},
//         }}, 
//     {{
//         {Square::O9 , Square::O11, Square::O12, Square::O10, {Square::O10, Square::O11, Square::O11}, {Square::O9 , Square::O10}}, 
//         {Square::O9 , Square::O7 , Square::O5 , Square::O8 , {Square::O6 , Square::O7 , Square::O8 }, {Square::O8 , Square::O9 }},
//         }}
//     }},
// {{
//     {{
//         {Square::I2 , Square::K2 , Square::L2 , Square::J2 , {Square::J2 , Square::K2 , Square::K2 }, {Square::I2 , Square::J2 }}, 
//         {Square::I2 , Square::G2 , Square::E2 , Square::H2 , {Square::F2 , Square::G2 , Square::H2 }, {Square::H2 , Square::I2 }},
//         }}, 
//     {{
//         {Square::B9 , Square::B11, Square::B12, Square::B10, {Square::B10, Square::B11, Square::B11}, {Square::B9 , Square::B10}}, 
//         {Square::B9 , Square::B7 , Square::B5 , Square::B8 , {Square::B6 , Square::B7 , Square::B8 }, {Square::B8 , Square::B9 }},
//         }}, 
//     {{
//         {Square::H15, Square::F15, Square::E15, Square::G15, {Square::F15, Square::G15, Square::G15}, {Square::G15, Square::H15}}, 
//         {Square::H15, Square::J15, Square::L15, Square::I15, {Square::I15, Square::J15, Square::K15}, {Square::H15, Square::I15}},
//         }}, 
//     {{
//         {Square::O8 , Square::O6 , Square::O5 , Square::O7 , {Square::O6 , Square::O7 , Square::O7 }, {Square::O7 , Square::O8 }}, 
//         {Square::O8 , Square::O10, Square::O12, Square::O9 , {Square::O9 , Square::O10, Square::O11}, {Square::O8 , Square::O9 }},
//         }}
//     }}
// }};

} // namespace athena