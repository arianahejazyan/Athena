#ifndef CHESS_H
#define CHESS_H

#include <cstdint>

namespace Talia
{

class Square
{
    private:

        uint8_t encoded;
    
    public:

        Square() noexcept : encoded(0) {}

        Square(uint8_t index) noexcept : encoded(index) {}

        // -------------------------------

        [[nodiscard]] int rank() const noexcept { return encoded >> 4;  } // encoded / 16
        [[nodiscard]] int file() const noexcept { return encoded & 0xF; } // encoded % 16

        bool isEdgeOrCorner() const 
        {
            int r = rank();
            int f = file();

            // Check if the square is on the edge
            if (r == 0 || r == 15 || f == 0 || f == 15)
                return true;

            // Check if the square is within one of the 4x4 corner
            bool NW = (r < 4  && f < 4);
            bool NE = (r < 4  && f > 11);
            bool SW = (r > 11 && f < 4);
            bool SE = (r > 11 && f > 11);

            return NW || NE || SW || SE;
        }

        // -------------------------------


//         Square(int r, int f) noexcept : encoded((r << 4) | f) {}

//         ~Square() noexcept = default;

//         // -------------------------------

//         bool operator==(const Square& other) const noexcept {
//             return (encoded == other.encoded);
//         }

//         // -------------------------------

//         [[nodiscard]] int rank() const noexcept { return encoded >> 4;  }
//         [[nodiscard]] int file() const noexcept { return encoded & 0xF; }

//         // [[nodiscard]] bool isOFFBOARD() const noexcept { return this == &OFFBOARD; }

//         // -------------------------------

        operator uint8_t() const noexcept { return encoded; }
        
//         // -------------------------------

//         void str();

//         // -------------------------------

        
};

// // Define the OFFBOARD square as a static constant
// const Square OFFBOARD(0, 0);



// // enum Square : uint8_t
// // {
// //     a1,  b1,  c1,  d1,  e1,  f1,  g1,  h1,  i1,  j1,  k1,  l1,  m1,  n1,
// //     a2,  b2,  c2,  d2,  e2,  f2,  g2,  h2,  i2,  j2,  k2,  l2,  m2,  n2,
// //     a3,  b3,  c3,  d3,  e3,  f3,  g3,  h3,  i3,  j3,  k3,  l3,  m3,  n3,
// //     a4,  b4,  c4,  d4,  e4,  f4,  g4,  h4,  i4,  j4,  k4,  l4,  m4,  n4,
// //     a5,  b5,  c5,  d5,  e5,  f5,  g5,  h5,  i5,  j5,  k5,  l5,  m5,  n5,
// //     a6,  b6,  c6,  d6,  e6,  f6,  g6,  h6,  i6,  j6,  k6,  l6,  m6,  n6,
// //     a7,  b7,  c7,  d7,  e7,  f7,  g7,  h7,  i7,  j7,  k7,  l7,  m7,  n7,
// //     a8,  b8,  c8,  d8,  e8,  f8,  g8,  h8,  i8,  j8,  k8,  l8,  m8,  n8,
// //     a9,  b9,  c9,  d9,  e9,  f9,  g9,  h9,  i9,  j9,  k9,  l9,  m9,  n9,
// //     a10, b10, c10, d10, e10, f10, g10, h10, i10, j10, k10, l10, m10, n10,
// //     a11, b11, c11, d11, e11, f11, g11, h11, i11, j11, k11, l11, m11, n11,
// //     a12, b12, c12, d12, e12, f12, g12, h12, i12, j12, k12, l12, m12, n12,
// //     a13, b13, c13, d13, e13, f13, g13, h13, i13, j13, k13, l13, m13, n13,
// //     a14, b14, c14, d14, e14, f14, g14, h14, i14, j14, k14, l14, m14, n14, OFFBOARD
// // };

// enum Piece : uint8_t
// {
//     Pawn   = 0,
//     Knight = 1,
//     Bishop = 2,
//     Rook   = 3,
//     Queen  = 4,
//     King   = 5,
// };

// enum Color : uint8_t
// {
//     Red    = 0,
//     Blue   = 1,
//     Yellow = 2,
//     Green  = 3,
// };

// class PlayerInfo
// {
//     bool alive;
//     int score;
// };

// enum GameMode : uint8_t
// {
//     Team = 0,
//     FFA  = 1,
// };

// enum GameSetup : uint8_t
// {
//     Modern  = 0,
//     Classic = 1,
// };

// class PieceClass
// {
//     private:

//         uint8_t encoded;

//     public:

//         PieceClass() noexcept {}

//         PieceClass(Piece piece, Color color) noexcept : encoded((piece << 3) + color) {}

//         [[nodiscard]] inline Piece toPiece() const noexcept
//         {
//             return static_cast<Piece>(encoded >> 2);
//         }

//         [[nodiscard]] inline Color toColor() const noexcept
//         {
//             return static_cast<Color>(encoded & 0b11);
//         }

//         // Special instances
//         static const PieceClass Stone;
//         static const PieceClass None;

//         [[nodiscard]] inline bool isStone() const noexcept
//         {
//             return this == &Stone;
//         }
    
//         [[nodiscard]] inline bool isNone() const noexcept
//         {
//             return this == &None;
//         }

//         [[nodiscard]] inline bool operator==(const PieceClass& other) const noexcept
//         {
//             return encoded == other.encoded;
//         }
    
//         [[nodiscard]] inline bool operator!=(const PieceClass& other) const noexcept
//         {
//             return encoded != other.encoded;
//         }
// };

// // Define static instances
// const PieceClass Stone = PieceClass();
// const PieceClass None  = PieceClass();

// // Define constants
// constexpr std::size_t RANK_NB = 14;
// constexpr std::size_t FILE_NB = 14;
// constexpr std::size_t PIECE_NB = 6;
// constexpr std::size_t COLOR_NB = 4;
// constexpr std::size_t SQUARE_NB = RANK_NB * FILE_NB;
// constexpr std::size_t PIECECLASS_NB = PIECE_NB * COLOR_NB;

} // namespace Talia

#endif