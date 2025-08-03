#ifndef CHESS_H
#define CHESS_H

#include <array>
#include <cstdint>

namespace athena
{

// #define IMPORT_PIECE_ENUMS static constexpr u8 P = Piece::P, Pawn = Piece::Pawn, Q = Piece::Q, Queen = Piece::Queen, B = Piece::B, Bishop = Piece::Bishop, R = Piece::R, Rook  = Piece::Rook , N = Piece::N, Knight = Piece::Knight, K = Piece::K, King = Piece::King, A = Piece::A, All = Piece::All, E = Piece::E, Empty = Piece::Empty;
// #define IMPORT_COLOR_ENUMS static constexpr u8 r = Color::r, Red  = Color::Red , b = Color::b, Blue  = Color::Blue , y = Color::y, Yellow = Color::Yellow, g = Color::g, Green = Color::Green, o = Color::o, Orange = Color::Orange, c = Color::c, Cyan = Color::Cyan, m = Color::m, Mix = Color::Mix, n = Color::n, None  = Color::None ;

using u64 = uint64_t; using i64 = int64_t;
using u32 = uint32_t; using i32 = int32_t;
using u16 = uint16_t; using i16 = int16_t;
using u8  = uint8_t ; using i8  = int8_t ;

class Square;     
class PieceClass; 

using SQ = Square;
using PC = PieceClass;

constexpr std::size_t BOARDSIZE = 160;
constexpr std::size_t RANK_NB = 16;
constexpr std::size_t FILE_NB = 16;
constexpr std::size_t SQUARE_NB = RANK_NB * FILE_NB;

constexpr std::size_t PIECE_NB = 8;
constexpr std::size_t COLOR_NB = 8;
constexpr std::size_t PIECECOLOR_NB = PIECE_NB * COLOR_NB;

constexpr std::size_t SIDE_NB = 2;
constexpr std::size_t GUILD_NB = 2;
// constexpr std::size_t SHIFT_NB = 12;
constexpr std::size_t SETUP_NB = 2;

constexpr std::size_t CHUNK_NB = 4;
// constexpr std::size_t MAX_PLAY = 32;
// constexpr std::size_t MAX_MOVES = 256;

constexpr std::size_t MOVEFLAG_NB = 2;
constexpr std::size_t MOVETYPE_NB = 8;

constexpr std::size_t PLAYER_NB = 4;

constexpr std::size_t CASTLE_NB = SIDE_NB * PLAYER_NB * SETUP_NB;

constexpr std::size_t CACHELINE_SIZE = 64;

class Square
{
    public:
    enum : u8 
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
        A16, B16, C16, D16, E16, F16, G16, H16, I16, J16, K16, L16, M16, N16, O16, P16, OFFBOARD = 0U
    };

    u8 encoded;

    inline constexpr Square() {}
    inline constexpr Square(u8 sq) : encoded(sq) {}
    inline constexpr Square(u8 row, u8 col) : encoded((row << 4) | col) {}

    inline constexpr bool operator==(const SQ& other) { return encoded == other.encoded; }
    inline constexpr bool operator!=(const SQ& other) { return encoded != other.encoded; }

    inline constexpr operator u8() const { return encoded; }

    inline constexpr auto rank() const { return encoded >> 4; }
    inline constexpr auto file() const { return encoded & 15; }

    inline constexpr auto chunk() const { return  encoded >> 6; }
    inline constexpr auto index() const { return  encoded & 63; }

    inline constexpr SQ& flipFULL() { encoded ^= 0xFF; return *this; }
    inline constexpr SQ& flipRANK() { encoded ^= 0xF0; return *this; }
    inline constexpr SQ& flipFILE() { encoded ^= 0x0F; return *this; }

    inline constexpr SQ operator+(int offset) const { return SQ(encoded + offset); }
};

class Piece
{
    public:
    enum : u8
    {
        P = 0, // Pawn   = 0,
        Q = 1, // Queen  = 1,
        B = 2, // Bishop = 2,
        R = 3, // Rook   = 3,
        N = 4, // Knight = 4,
        K = 5, // King   = 5,
        A = 6, // All    = 6,
        E = 7, // Empty  = 7,
    };

    u8 encoded;

    inline constexpr Piece() {}
    inline constexpr Piece(u8 piece) : encoded(piece) {}

    inline constexpr bool operator==(const Piece& other) { return encoded == other.encoded; }
    inline constexpr bool operator!=(const Piece& other) { return encoded != other.encoded; }

    inline constexpr operator u8() const { return encoded; }
};

class Color
{
    public:
    enum : u8
    {
        r = 0, // Red    = 0,
        b = 1, // Blue   = 1,
        y = 2, // Yellow = 2,
        g = 3, // Green  = 3,
        o = 4, // Orange = 4,
        c = 5, // Cyan   = 5,
        m = 6, // Mix    = 6,
        n = 7, // None   = 7,
    };

    u8 encoded;

    inline constexpr Color() {}
    inline constexpr Color(u8 color) : encoded(color) {}

    inline constexpr bool operator==(const Color& other) { return encoded == other.encoded; }
    inline constexpr bool operator!=(const Color& other) { return encoded != other.encoded; }    

    inline constexpr operator u8() const { return encoded; }

    inline constexpr auto next() const { return Color((encoded + 1) & 0b011); }
    inline constexpr auto prev() const { return Color((encoded + 3) & 0b011); }
    inline constexpr auto ally() const { return Color((encoded    ) ^ 0b010); }
    inline constexpr auto team() const { return Color((encoded & 1) ^ 0b100); }
    inline constexpr auto enem() const { return Color((encoded & 1) ^ 0b101); }
};

class PieceClass
{
    public:
    enum : u8 { ORANGE = 56U, CYAN = 57U, EVERYTHING = 58U, STONE = 62U, NOTHING = 63U };

    u8 encoded;

    inline constexpr PieceClass() {}
    inline constexpr PieceClass(u8 pc) : encoded(pc) {}
    inline constexpr PieceClass(Piece piece, Color color) : encoded((color.encoded << 3) | piece.encoded) {}

    inline constexpr bool operator==(const PC& other) { return encoded == other.encoded; }
    inline constexpr bool operator!=(const PC& other) { return encoded != other.encoded; }

    inline constexpr operator u8() const { return encoded; }

    inline constexpr auto piece() const { return Piece(encoded &  7); }
    inline constexpr auto color() const { return Color(encoded >> 3); }
    // inline constexpr auto guild() const { return static_cast<u8   >(encoded &  1); }
};


    // enum : u8
    // {
    //     rP = (r << 3) | P, bP = (b << 3) | P, yP = (y << 3) | P, gP = (g << 3) | P, oP = (o << 3) | P, cP = (c << 3) | P,
    //     rQ = (r << 3) | Q, bQ = (b << 3) | Q, yQ = (y << 3) | Q, gQ = (g << 3) | Q, oQ = (o << 3) | Q, cQ = (c << 3) | Q,
    //     rB = (r << 3) | B, bB = (b << 3) | B, yB = (y << 3) | B, gB = (g << 3) | B, oB = (o << 3) | B, cB = (c << 3) | B,
    //     rR = (r << 3) | R, bR = (b << 3) | R, yR = (y << 3) | R, gR = (g << 3) | R, oR = (o << 3) | R, cR = (c << 3) | R,
    //     rN = (r << 3) | N, bN = (b << 3) | N, yN = (y << 3) | N, gN = (g << 3) | N, oN = (o << 3) | N, cN = (c << 3) | N,
    //     rK = (r << 3) | K, bK = (b << 3) | K, yK = (y << 3) | K, gK = (g << 3) | K, oK = (o << 3) | K, cK = (c << 3) | K,

    //     ORANGE = 56U, CYAN = 57U, EVERYTHING = 58U, STONE = 62U, NOTHING = 63U,
    // };


enum Setup : u8 // make class
{ 
    Modern  = 0, 
    Classic = 1,
};

enum Side : u8 // make class
{
    KS = 0, // KingSide  = 0,
    QS = 1, // QueenSide = 1,
};

class Rights
{
    // IMPORT_COLOR_ENUMS
    public:
    // enum : u8
    // {
    //     rKS = (1 << r) << (KS * 4), rQS = (1 << r) << (QS * 4),
    //     bKS = (1 << b) << (KS * 4), bQS = (1 << b) << (QS * 4),
    //     yKS = (1 << y) << (KS * 4), yQS = (1 << y) << (QS * 4),
    //     gKS = (1 << g) << (KS * 4), gQS = (1 << g) << (QS * 4),

    //     rKQ = rKS ^ rQS,
    //     bKQ = bKS ^ bQS,
    //     yKQ = yKS ^ yQS,
    //     gKQ = gKS ^ gQS,
        
    //     RIGHTS = rKQ ^ bKQ ^ yKQ ^ gKQ,
    // };

    u8 encoded;

    inline constexpr Rights() {}
    inline constexpr Rights(u8 rights)   : encoded(rights) {}
    inline constexpr Rights(Color color) : encoded(0x11 << color) {}
    inline constexpr Rights(Color color, Side side) : encoded(0x1 << (side * 4 + color)) {}

    // inline constexpr operator u8() const { return encoded; }

    inline constexpr bool operator==(const Rights& other) { return encoded == other.encoded; }
    inline constexpr bool operator!=(const Rights& other) { return encoded != other.encoded; }

    inline constexpr bool operator()(Color color) const { return (encoded >> color) & 0x11;}
    inline constexpr bool operator()(Color color, Side side) const { return (encoded >> (side * 4 + color)) & 0x1;}



    // inline constexpr auto encode(Color color, Side side)       { encoded ^= (0x11 << color.encoded) & (0xF << (side * 4)); }
    // inline constexpr auto decode(Color color, Side side) const { return  (encoded >> color.encoded) & (0x1 << (side * 4)); }

    // inline constexpr auto encode(Color color)       { encoded ^= (0x11 << color.encoded)       ; }
    // inline constexpr auto decode(Color color) const { return  (encoded >> color.encoded) & 0x11; }
};


// enum  : uint8_t
// {
//     rKS = (1 << r) << (KS * 4), rQS = (1 << r) << (QS * 4),
//     bKS = (1 << b) << (KS * 4), bQS = (1 << b) << (QS * 4),
//     yKS = (1 << y) << (KS * 4), yQS = (1 << y) << (QS * 4),
//     gKS = (1 << g) << (KS * 4), gQS = (1 << g) << (QS * 4),

//     rKQ = rKS | rQS,
//     bKQ = bKS | bQS,
//     yKQ = yKS | yQS,
//     gKQ = gKS | gQS,
    
//     RIGHTS = rKQ | bKQ | yKQ | gKQ,
// };


// inline constexpr auto encodeCastle(GameSetup setup, Color color, Side side) {
//     return static_cast<Rights>((setup << 3) | (color << 1) | side);
// }



// class Pack
// {
//     private:
//     std::array<SQ, PLAYER_NB> packed;

//     public:
//     Pack() {}

//     inline       SQ& operator[](Color color)       { return packed[color.encoded]; }
//     inline const SQ& operator[](Color color) const { return packed[color.encoded]; }

// //     inline constexpr SQ operator[](Color c) const {
// //     return packed[c.encoded];
// // }

// };


class Pack
{
    public:
    u32 packed;

    inline Pack() {}
    inline Pack(u32 pack) : packed(pack) {}
    inline Pack(SQ sq, Color color) : packed(sq << 8) {}

    inline auto operator[](Color color) const { return static_cast<SQ>((packed >> 8) & 0xFF); }

    // inline      SQ& operator[](Color color)       { return packed[color.encoded]; }
    // inline const SQ& operator[](Color color) const { return packed[color.encoded]; }

//     inline constexpr SQ operator[](Color c) const {
//     return packed[c.encoded];
// }

};




// enum PieceColor : uint8_t
// {
//     rP = r | (P << 3), bP = b | (P << 3), yP = y | (P << 3), gP = g | (P << 3), oP = o | (P << 3), cP = c | (P << 3),
//     rN = r | (N << 3), bN = b | (N << 3), yN = y | (N << 3), gN = g | (N << 3), oN = o | (N << 3), cN = c | (N << 3),
//     rB = r | (B << 3), bB = b | (B << 3), yB = y | (B << 3), gB = g | (B << 3), oB = o | (B << 3), cB = c | (B << 3),
//     rR = r | (R << 3), bR = b | (R << 3), yR = y | (R << 3), gR = g | (R << 3), oR = o | (R << 3), cR = c | (R << 3),
//     rQ = r | (Q << 3), bQ = b | (Q << 3), yQ = y | (Q << 3), gQ = g | (Q << 3), oQ = o | (Q << 3), cQ = c | (Q << 3),
//     rK = r | (K << 3), bK = b | (K << 3), yK = y | (K << 3), gK = g | (K << 3), oK = o | (K << 3), cK = c | (K << 3),

//     GUILD1 = o | (A << 3),
//     GUILD2 = c | (A << 3), ALL = m | (A << 3), STONE = n | (A << 3), NOTHING = n | (E << 3),
// };

// enum PieceColor : uint8_t
// {
//     rP = (r << 3) | P, bP = (b << 3) | P, yP = (y << 3) | P, gP = (g << 3) | P, oP = (o << 3) | P, cP = (c << 3) | P,
//     rN = (r << 3) | N, bN = (b << 3) | N, yN = (y << 3) | N, gN = (g << 3) | N, oN = (o << 3) | N, cN = (c << 3) | N,
//     rB = (r << 3) | B, bB = (b << 3) | B, yB = (y << 3) | B, gB = (g << 3) | B, oB = (o << 3) | B, cB = (c << 3) | B,
//     rR = (r << 3) | R, bR = (b << 3) | R, yR = (y << 3) | R, gR = (g << 3) | R, oR = (o << 3) | R, cR = (c << 3) | R,
//     rQ = (r << 3) | Q, bQ = (b << 3) | Q, yQ = (y << 3) | Q, gQ = (g << 3) | Q, oQ = (o << 3) | Q, cQ = (c << 3) | Q,
//     rK = (r << 3) | K, bK = (b << 3) | K, yK = (y << 3) | K, gK = (g << 3) | K, oK = (o << 3) | K, cK = (c << 3) | K,

//     OCC1 = (n << 3) | P,
//     OCC2 = (n << 3) | N,
//     OCC3 = (n << 3) | B,

//     STONE = (n << 3) | A, NOTHING = (n << 3) | E,
// };




// enum Delta : int
// {
//     Z  =  0   ,
//     N  = +16  ,
//     S  = -16  ,
//     E  = +1   ,
//     W  = -1   ,
//     NE = N + E,
//     NW = N + W,
//     SE = S + E,
//     SW = S + W,
//     NN = N + N,
//     SS = S + S,
//     EE = E + E,
//     WW = W + W,
// };

// enum Side : uint8_t
// {
//     KS = 0,
//     QS = 1,
// };

// enum Rights : uint8_t
// {
//     rKS = (1 << r) << (KS * 4), rQS = (1 << r) << (QS * 4),
//     bKS = (1 << b) << (KS * 4), bQS = (1 << b) << (QS * 4),
//     yKS = (1 << y) << (KS * 4), yQS = (1 << y) << (QS * 4),
//     gKS = (1 << g) << (KS * 4), gQS = (1 << g) << (QS * 4),

//     rKQ = rKS | rQS,
//     bKQ = bKS | bQS,
//     yKQ = yKS | yQS,
//     gKQ = gKS | gQS,
    
//     RIGHTS = rKQ | bKQ | yKQ | gKQ,
// };






// inline constexpr auto encodeSQ(int rank, int file) {
//     return static_cast<SQ>((rank << 4) | file);  
// }

// inline constexpr auto decodeRank(SQ sq) { return static_cast<uint8_t>(sq >> 4); }
// inline constexpr auto decodeFile(SQ sq) { return static_cast<uint8_t>(sq & 15); }

// inline constexpr auto rankSQ(SQ sq) { return sq >> 4; }
// inline constexpr auto fileSQ(SQ sq) { return sq & 0xF; }
// inline constexpr auto makeSQ(int r, int f)  { return static_cast<SQ>((r << 4) | f);    }

// inline constexpr auto mirrorRank(SQ sq) { return static_cast<SQ>(sq ^ 0xF0); }
// inline constexpr auto mirrorFile(SQ sq) { return static_cast<SQ>(sq ^ 0x0F); }

// inline constexpr auto chunkSQ(SQ sq) { return sq >> 6; }
// inline constexpr auto indexSQ(SQ sq) { return sq & 63; }
// inline constexpr auto layerSQ(uint8_t index) { return static_cast<uint8_t>(index >> 4); }

inline constexpr bool isInValidSquare(int r, int f)
{
    return (r <= 0  || r >= 15) ||
           (f <= 0  || f >= 15) ||
           (r <= 3  && f <= 3 ) ||
           (r <= 3  && f >= 12) ||
           (r >= 12 && f <= 3 ) ||
           (r >= 12 && f >= 12);
}



template<std::size_t chunk>
inline SQ compress(SQ sq)
{
    static_assert(chunk < 4,
                 "compress: invalid chunk index");

    if constexpr (chunk == 0) return sq.encoded - (12 + (sq.index() >> 4) * 8);
    if constexpr (chunk == 1) return sq.encoded - (41 + (sq.index() >> 4) * 2);
    if constexpr (chunk == 2) return sq.encoded - (49 + (sq.index() >> 4) * 2);
    if constexpr (chunk == 3) return sq.encoded - (60 + (sq.index() >> 4) * 8);
}

template<std::size_t chunk>
inline SQ decompress(SQ sq)
{
    static_assert(chunk < 4,
                 "decompress: invalid chunk index");

    if constexpr (chunk == 0) return sq.encoded + (12 + (sq.index() >> 4) * 8);
    if constexpr (chunk == 1) return sq.encoded + (41 + (sq.index() >> 4) * 2);
    if constexpr (chunk == 2) return sq.encoded + (49 + (sq.index() >> 4) * 2);
    if constexpr (chunk == 3) return sq.encoded + (60 + (sq.index() >> 4) * 8);
}

// inline constexpr auto toPiece(PC pc) {
//     return static_cast<Piece>(pc >> 3);
// }

// inline constexpr auto toColor(PC pc) {
//     return static_cast<Color>(pc & 0x7);
// }

// inline constexpr auto toPieceColor(Piece piece, Color color) {
//     return static_cast<PieceColor>(color | (piece << 3));
// }

// inline constexpr auto decodePiece(PC pc) {
//     return static_cast<Piece>(pc & 0x7);
// }

// inline constexpr auto decodeColor(PC pc) {
//     return static_cast<Color>(pc >> 3);
// }

// inline constexpr auto encodePC(Color color, Piece piece) {
//     return static_cast<PC>((color << 3) | piece);
// }

// inline constexpr auto teammate(Color color) {
//     return static_cast<Color>((color & 0b001) | 0b100);
// }

// inline constexpr auto opponent(Color color) {
//     return static_cast<Color>((color & 0b001) ^ 0b101);
// }

// inline constexpr auto next(Color color)  {
//     return static_cast<Color>((color + 1) & 0b11);
// }

// inline constexpr auto prev(Color color) {
//     return static_cast<Color>((color + 3) & 0b11);
// }

// inline constexpr auto ally(Color color) {
//     return static_cast<Color>(color ^ 0b10);
// }

// inline constexpr SQ operator+(SQ sq, int delta) {
//     return static_cast<SQ>(static_cast<int>(sq) + static_cast<int>(delta));
// }

// inline constexpr SQ operator+(SQ sq, Delta delta) {
//     return static_cast<SQ>(static_cast<int>(sq) + static_cast<int>(delta));
// }

// inline constexpr SQ operator-(SQ sq, Delta delta) {
//     return static_cast<SQ>(static_cast<int>(sq) - static_cast<int>(delta));
// }

// toGuild
// toSetup, toSide, toColor, toCastle 

// inline constexpr uint8_t mapGuild(Color color) {
//     return static_cast<uint8_t>(1 << ((color & 0x1) * 2));
// }

// inline constexpr uint8_t mapColor(Color color) {
//     return static_cast<uint8_t>(color + ((color & 0x1) * 2));
// }


// inline constexpr auto indexGuild(Color color) {
//     return static_cast<uint8_t>(1 << ((color & 0x1) << 2));
// }

// inline constexpr auto indexColor(Color color) {
//     return static_cast<uint8_t>(color + ((color & 0x1) * 2));
// }

//return static_cast<uint8_t>(color ^ ((color & 0x1) * (0b101)));

/*
    BitBoard Layout
    (x, r) (x, o) (x, y) garbage (x, b) (x, c) (x, g) garbage
    (a, o) (a, m) (a, c)
    use chunkPC, chunkAll, indexPC, indexAll
*/

// // roybcg
// inline constexpr auto chunkPC(Color color) {
//     return static_cast<uint8_t>((color >> 1) | ((color & 0x1) << 2));
// }

// // 1a2
// inline constexpr auto chunkAll(Color color) {
//     return static_cast<uint8_t>(color & 0x1);
// }

// inline constexpr auto decodeGuild(Color color) {
//     return static_cast<Color>(color & 0x1);
// }

// inline constexpr auto decodeGuild(PC pc) {
//     return static_cast<Color>((pc & 0x1) | 0b100);
// }

// inline constexpr auto chunkGuild(Piece piece, Color color) {
//     return encodePC(piece, static_cast<Color>(color & 0x1));
// }

// inline constexpr auto indexGuild(Color color) {
//     return static_cast<Color>((color & 0x1) + 4);
// }

// inline constexpr auto chunkGuild(Color color) {
//     return static_cast<Color>(color & 0x1);
// }

// inline constexpr auto occGuild(Color color) {
//     return static_cast<Color>((n << 3) | (color & 0x1));
// }






// inline constexpr auto encodeEnpass(SQ sq, Color color) {
//     return static_cast<uint32_t>(sq << (color * 8));
// }

// inline constexpr auto decodeEnpass(uint32_t enpass, Color color) {
//     return static_cast<SQ>((enpass >> (color * 8)) & 0xFF);
// }

// inline constexpr auto encodeRoyal(SQ sq, Color color) {
//     return static_cast<uint32_t>(sq << (color * 8));
// }

// inline constexpr auto decodeRoyal(uint32_t royals, Color color) {
//     return static_cast<SQ>((royals >> (color * 8)) & 0xFF);
// }

// inline constexpr auto encodeCastle(GameSetup setup, Color color, Side side) {
//     return static_cast<Rights>((setup << 3) | (color << 1) | side);
// }

// inline constexpr auto encodeCastle(Color color, Side side) {
//     return static_cast<Rights>((0x1 << color) << (side * 4));
// }

// inline constexpr auto encodeCastle(Color color) {
//     return static_cast<Rights>(0x11 << color);
// }


// inline constexpr auto mirrorChunk(uint8_t chunk) {
//     return static_cast<uint8_t>((CHUNK_NB - 1) - chunk);
// }

// char* toString(char* out, Piece piece, bool lowercase = false);
// char* toString(char* out, Color color, bool uppercase = false);

// const Color toColor(char color);
// const Piece toPiece(char piece);


// inline constexpr Rights toRights(Color color) {
//     return static_cast<Rights>(0x3 << (color << 1));
// }

// inline constexpr Rights toRights(Color color, Side side) {
//     return static_cast<Rights>((0x1 << (color << 1)) << side);
// }


// const Rights toRights(char* s, Side side);


// char* toString(char* out, Rights rights, Side side);
// const char toString(Piece piece);
// const char toString(Color color);
// const std::string toString(PieceClass pc);


// const PieceClass toPieceClass(const std::string& pc);

// const auto toString(GameSetup setup);
// const auto toGameSetup(const std::string& setup);

// const auto toString(SQ sq, bool debug = false);
// const auto toSquare(const std::string &sq, bool debug = false);

class MoveType;
// class MoveFlag;


using Type = MoveType;

class MoveType
{
    public:
    enum : u8
    {
        Jumper = 0, Slider = 1,
        Pushed = 2, Strike = 3,
        Stride = 4, Enpass = 5,
        Castle = 6, Evolve = 7,
    };

    u8 encoded;
    inline constexpr MoveType(u8 type) : encoded(type) {}

    inline constexpr operator u8() const { return encoded; }
    inline constexpr bool operator==(const MoveType& other) { return encoded == other.encoded; }
    inline constexpr bool operator!=(const MoveType& other) { return encoded != other.encoded; }

    inline bool special() const { return encoded >= Stride; }
};


constexpr std::size_t MOVES_NB = 256;

enum MoveFlag : u8
{
    Noisy = 0,
    Quiet = 1,
};
using Flag = MoveFlag;
// enum MoveType : u8
// {
//     Jumper = 0,
//     Slider = 1,
//     Pushed = 2,
//     Strike = 3,
//     Stride = 4,
//     Enpass = 5,
//     Castle = 6,
//     Evolve = 7,
// };



class Move
{
    private:
    uint32_t encoded;

    public:
    inline constexpr Move() {}

    inline constexpr Move(SQ source, SQ target, MoveType type, MoveFlag flag)
        : encoded(source.encoded ^ (target.encoded << 24) ^ (type         << 8) ^ (flag  << 11)) {}

    inline constexpr Move(SQ source, SQ target, Color color)
        : encoded(source.encoded ^ (target.encoded << 24) ^ (Type::Enpass << 8) ^ (Noisy << 11) ^ (color.encoded << 12)) {}

    inline constexpr Move(SQ source, SQ target, PC pc)
        : encoded(source.encoded ^ (target.encoded << 24) ^ (Type::Evolve << 8) ^ (Noisy << 11) ^ (pc.encoded << 15)) {}

    inline consteval Move(SQ source, SQ target, Side side)
        : encoded(source.encoded ^ (target.encoded << 24) ^ (Type::Castle << 8) ^ (Quiet << 11) ^ (side << 23)) {}   

    inline constexpr auto source() const {
        return static_cast<SQ>(encoded & 0xFF);
    }

    inline constexpr auto target() const {
        return static_cast<SQ>(encoded >> 24);
    }

    inline constexpr auto enpass() const {
        return static_cast<Color>((encoded >> 12) & 0x7);
    }

    inline constexpr auto evolve() const {
        return static_cast<PC>((encoded >> 15) & 0xFF);
    }

    inline constexpr auto castle() const {
        return static_cast<Side>((encoded >> 23) & 0x1);
    }

    inline constexpr auto type() const {
        return static_cast<Type>((encoded >> 8 ) & 0x7);
    }

    inline constexpr auto flag() const {
        return static_cast<Flag>((encoded >> 11) & 0x1);
    }

    const char* uci(bool debug = false) const;
};

} // namespace athena

#endif /* CHESS_H */