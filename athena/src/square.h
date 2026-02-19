#pragma once

#include <string>
#include <string_view>
#include <cstdint>
#include "constants.h"
#include "offset.h"
#include "color.h"

namespace athena
{

class Square
{
    public: enum : uint8_t 
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

    public:

    // Default constructor
    constexpr Square() = default;

    // Constructor
    constexpr Square(uint8_t value)
        : value_(value) {}

    // Coordinate constructor
    constexpr Square(int rank, int file)
        : value_(rank * FILE_NB + file) {}

    // UCI Constructor
    Square(std::string_view square, bool expanded = false) noexcept;

    // Index & Chunk operators
    inline constexpr uint8_t chunk() const noexcept { return value_ >> 6; } // value_ / CHUNK_SIZE
    inline constexpr uint8_t index() const noexcept { return value_ & 63; } // value_ % CHUNK_SIZE
    
    // Rank & File operators
    inline constexpr int rank() const noexcept { return value_ >> 4; } // value_ / FILE_NB 
    inline constexpr int file() const noexcept { return value_ & 15; } // value_ % FILE_NB

    // Offboard operator
    inline constexpr bool offboard() const noexcept { return value_ == Offboard; }

    // Stone operator
    inline constexpr bool stone() const noexcept;

    // Conversion operator
    explicit constexpr operator uint8_t() const {
        return value_;
    }

    // Used to check if pawns can double push
    template<uint8_t color>
    constexpr bool homerank() const noexcept;

    // Used to check if pawns can promote
    template<uint8_t color>
    constexpr bool promotion() const noexcept;

    // Return the UCI representation of the square
    std::string uci(bool expanded = false) const noexcept;

    private:
    uint8_t value_;
};

// ===== function declarations ===== //

inline constexpr bool Square::stone() const noexcept
{
    auto r = rank();
    auto f = file();

    return 
    (r == 0  || f == 0 ) || (r <= 3  && f <= 3 ) || (r >= 12 && f >= 12) ||
    (r == 15 || f == 15) || (r <= 3  && f >= 12) || (r >= 12 && f <= 3 );    
}

// ===== operators ===== //

inline constexpr Square operator+(Square sq, Offset offset) noexcept {
    return static_cast<Square>(static_cast<uint8_t>(sq) + static_cast<int>(offset));
}

inline constexpr Square operator-(Square sq, Offset offset) noexcept {
    return static_cast<Square>(static_cast<uint8_t>(sq) - static_cast<int>(offset));
}

inline constexpr Square& operator+=(Square& sq, Offset offset) noexcept {
    sq = sq + offset;
    return sq;
}

inline constexpr Square& operator-=(Square& sq, Offset offset) noexcept {
    sq = sq - offset;
    return sq;
}

inline constexpr Square operator+(Square lhs, Square rhs) noexcept {
    return static_cast<Square>(static_cast<uint8_t>(lhs) + static_cast<uint8_t>(rhs));
}

inline constexpr Square operator-(Square lhs, Square rhs) noexcept {
    return static_cast<Square>(static_cast<uint8_t>(lhs) - static_cast<uint8_t>(rhs));
}

inline constexpr Square& operator++(Square& sq) noexcept {
    sq = static_cast<Square>(static_cast<uint8_t>(sq) + 1);
    return sq;
}

inline constexpr Square operator++(Square& sq, int) noexcept {
    Square old = sq;
    ++sq;
    return old;
}

inline constexpr Square& operator--(Square& sq) noexcept {
    sq = static_cast<Square>(static_cast<uint8_t>(sq) - 1);
    return sq;
}

inline constexpr Square operator--(Square& sq, int) noexcept {
    Square old = sq;
    --sq;
    return old;
}

inline constexpr bool operator<(Square lhs, Square rhs) noexcept {
    return static_cast<uint8_t>(lhs) < static_cast<uint8_t>(rhs);
}

inline constexpr bool operator<=(Square lhs, Square rhs) noexcept {
    return static_cast<uint8_t>(lhs) <= static_cast<uint8_t>(rhs);
}

inline constexpr bool operator>(Square lhs, Square rhs) noexcept {
    return static_cast<uint8_t>(lhs) > static_cast<uint8_t>(rhs);
}

inline constexpr bool operator>=(Square lhs, Square rhs) noexcept {
    return static_cast<uint8_t>(lhs) >= static_cast<uint8_t>(rhs);
}

inline constexpr bool operator==(Square lhs, Square rhs) noexcept {
    return static_cast<uint8_t>(lhs) == static_cast<uint8_t>(rhs);
}

inline constexpr bool operator!=(Square lhs, Square rhs) noexcept {
    return static_cast<uint8_t>(lhs) != static_cast<uint8_t>(rhs);
}

inline constexpr Square operator<<(Square lhs, int rhs) noexcept {
    return static_cast<Square>(static_cast<uint8_t>(lhs) << rhs);
}

inline constexpr Square operator>>(Square lhs, int rhs) noexcept {
    return static_cast<Square>(static_cast<uint8_t>(lhs) >> rhs);
}

inline constexpr Square& operator<<=(Square& lhs, int rhs) noexcept {
    lhs = lhs << rhs;
    return lhs;
}

inline constexpr Square& operator>>=(Square& lhs, int rhs) noexcept {
    lhs = lhs >> rhs;
    return lhs;
}

// ====== homerank (template specialization) ====== //

template<>
inline bool Square::homerank<Color::Red>() const noexcept {
    return rank() == HOME_RANK || rank() == HOME_RANK - 1;
}

template<>
inline bool Square::homerank<Color::Blue>() const noexcept {
    return file() == HOME_RANK || file() == HOME_RANK - 1;
}

template<>
inline bool Square::homerank<Color::Yellow>() const noexcept {
    return rank() == (RANK_NB - HOME_RANK) || rank() == (RANK_NB - HOME_RANK - 1);
}

template<>
inline bool Square::homerank<Color::Green>() const noexcept {
    return file() == (FILE_NB - HOME_RANK) || file() == (FILE_NB - HOME_RANK - 1);
}

// ====== promotion (template specialization) ====== //

template<>
inline bool Square::promotion<Color::Red>() const noexcept {
    return rank() == PROMOTION_RANK;
}

template<>
inline bool Square::promotion<Color::Blue>() const noexcept {
    return file() == PROMOTION_RANK;
}

template<>
inline bool Square::promotion<Color::Yellow>() const noexcept {
    return rank() == (RANK_NB - PROMOTION_RANK - 1);
}

template<>
inline bool Square::promotion<Color::Green>() const noexcept {
    return file() == (FILE_NB - PROMOTION_RANK - 1);
}

} // namespace athena