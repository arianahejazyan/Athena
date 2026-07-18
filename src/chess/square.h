#pragma once

#include <array>
#include <cstddef>
#include <string>
#include <string_view>
#include <cstdint>
#include "piece.h"
#include "color.h"
#include "constants.h"

namespace athena::chess {

class Square {
public: 
    enum class ID : uint8_t {
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
        A16, B16, C16, D16, E16, F16, G16, H16, I16, J16, K16, L16, M16, N16, O16, P16,
    };

    enum class Offset : int {
        North = +16,
        South = -16,
        East  = +1 ,
        West  = -1 , 
        None  =  0 ,
    };

    using Index = uint8_t;
    using Chunk = uint8_t;
    using Rank  = uint8_t;
    using File  = uint8_t;

    constexpr Square() noexcept = default;
    constexpr Square(ID id) noexcept : id_(id) {}
    constexpr Square(File file, Rank rank) noexcept : id_(static_cast<ID>(file + FILE_NB * rank)) {}

    Square(std::string_view sv, bool board16x16 = false) noexcept {
        int rank = std::stoi(std::string(sv.substr(1))); 
        int file = sv[0] - 'a' + 1;
        if (board16x16) {
            rank--;
            file--;
        }
        *this = Square(static_cast<uint8_t>(file), static_cast<uint8_t>(rank));
    }

    std::string uci(bool board16x16 = false) const noexcept {
        int r = rank();
        int f = file() - 1;
        if (board16x16) {
            r++;
            f++;
        }
        return std::string(1, static_cast<char>('a' + f)) + std::to_string(r);
    }

    constexpr bool isValid() const noexcept { return !isStone(); }  
    constexpr bool isStone() const noexcept {
        auto r = rank();
        auto f = file();
        return 
            (r <= 0  || f <= 0 ) || 
            (r <= 3  && f <= 3 ) || 
            (r >= 12 && f >= 12) ||
            (r >= 15 || f >= 15) || 
            (r <= 3  && f >= 12) || 
            (r >= 12 && f <= 3 );   
    }

    constexpr Index index() const noexcept { return static_cast<uint8_t>(id_) & 63; } 
    constexpr Chunk chunk() const noexcept { return static_cast<uint8_t>(id_) >> 6; } 
    constexpr Rank  rank()  const noexcept { return static_cast<uint8_t>(id_) >> 4; } 
    constexpr File  file()  const noexcept { return static_cast<uint8_t>(id_) & 15; } 

    constexpr Square flip_file() { return static_cast<ID>(static_cast<uint8_t>(id_) ^ 0x0F); }
    constexpr Square flip_rank() { return static_cast<ID>(static_cast<uint8_t>(id_) ^ 0xF0); }
    constexpr Square flip_both() { return static_cast<ID>(static_cast<uint8_t>(id_) ^ 0xFF); }

    constexpr bool homerank(Color::ID color) const noexcept;
    constexpr bool promotes(Color::ID color) const noexcept;

    constexpr ID id() const noexcept { return id_; }
    constexpr std::size_t compact() const noexcept { return static_cast<std::size_t>(static_cast<uint8_t>(id_) - 20); }

    constexpr Square compress() const noexcept {
        alignas(64) constexpr static std::array<uint8_t, SQUARE_NB> table = {
            0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
            0,   0,   0,   0,   0,   1,   2,   3,   4,   5,   6,   7,   0,   0,   0,   0,
            0,   0,   0,   0,   8,   9,  10,  11,  12,  13,  14,  15,   0,   0,   0,   0,
            0,   0,   0,   0,  16,  17,  18,  19,  20,  21,  22,  23,   0,   0,   0,   0,
            0,  24,  25,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  37,   0,
            0,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51,   0,
            0,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,   0,
            0,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,   0,
            0,  80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  91,  92,  93,   0,
            0,  94,  95,  96,  97,  98,  99, 100, 101, 102, 103, 104, 105, 106, 107,   0,
            0, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121,   0,
            0, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135,   0,
            0,   0,   0,   0, 136, 137, 138, 139, 140, 141, 142, 143,   0,   0,   0,   0,
            0,   0,   0,   0, 144, 145, 146, 147, 148, 149, 150, 151,   0,   0,   0,   0,
            0,   0,   0,   0, 152, 153, 154, 155, 156, 157, 158, 159,   0,   0,   0,   0,
            0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      };
        return static_cast<Square::ID>(table[static_cast<uint8_t>(id_)]);
    }
    
private:
    ID id_;

public:
    template<Piece::ID piece>
    static constexpr auto offsets() noexcept;

    template<Color::ID color>
    static constexpr auto offsets() noexcept;

    static constexpr Offset push(Color::ID color, int idx) noexcept;
    static constexpr Offset take(Color::ID color, int idx) noexcept;

    static constexpr Square middle(Square a, Square b) noexcept {
        return static_cast<Square::ID>((static_cast<uint8_t>(a.id_) + static_cast<uint8_t>(b.id_)) >> 1
        );
    }

    static constexpr Square::ID offboard() { return Square::ID::A1; }
};

inline constexpr bool Square::homerank(Color::ID color) const noexcept {
    switch (color) {
        case Color::ID::Red   : return (rank()               == HOMERANK) || (          rank()     == HOMERANK - 1);
        case Color::ID::Blue  : return (file()               == HOMERANK) || (          file()     == HOMERANK - 1);
        case Color::ID::Yellow: return (RANK_NB - rank() - 1 == HOMERANK) || (RANK_NB - rank() - 1 == HOMERANK - 1);
        case Color::ID::Green : return (FILE_NB - file() - 1 == HOMERANK) || (FILE_NB - file() - 1 == HOMERANK - 1);
        default: return false;
    }
}

inline constexpr bool Square::promotes(Color::ID color) const noexcept {
    switch (color) {
        case Color::ID::Red   : return (rank()               == PROMOTES);
        case Color::ID::Blue  : return (file()               == PROMOTES);
        case Color::ID::Yellow: return (RANK_NB - rank() - 1 == PROMOTES);
        case Color::ID::Green : return (FILE_NB - file() - 1 == PROMOTES);
        default: return false;
    }
}

inline constexpr bool operator==(Square lhs, Square rhs) noexcept { return lhs.id() == rhs.id(); }
inline constexpr bool operator!=(Square lhs, Square rhs) noexcept { return lhs.id() != rhs.id(); }

inline constexpr Square operator+(Square lhs, Square::Offset rhs) noexcept { return static_cast<Square::ID>(static_cast<uint8_t>(lhs.id()) + static_cast<int>(rhs)); }
inline constexpr Square operator-(Square lhs, Square::Offset rhs) noexcept { return static_cast<Square::ID>(static_cast<uint8_t>(lhs.id()) - static_cast<int>(rhs)); }

inline constexpr Square::ID operator+(Square::ID lhs, Square::Offset rhs) noexcept { return static_cast<Square::ID>(static_cast<uint8_t>(lhs) + static_cast<int>(rhs)); }
inline constexpr Square::ID operator-(Square::ID lhs, Square::Offset rhs) noexcept { return static_cast<Square::ID>(static_cast<uint8_t>(lhs) - static_cast<int>(rhs)); }

inline constexpr Square::Offset operator+(Square::Offset lhs, Square::Offset rhs) noexcept { return static_cast<Square::Offset>(static_cast<int>(lhs) + static_cast<int>(rhs)); }
inline constexpr Square::Offset operator-(Square::Offset lhs, Square::Offset rhs) noexcept { return static_cast<Square::Offset>(static_cast<int>(lhs) - static_cast<int>(rhs)); }

inline constexpr Square& operator+=(Square& sq, Square::Offset offset) noexcept { sq = sq + offset; return sq; }
inline constexpr Square& operator-=(Square& sq, Square::Offset offset) noexcept { sq = sq - offset; return sq; }

inline constexpr bool operator>(Square::Offset lhs, int rhs) noexcept { return static_cast<int>(lhs) > rhs; }
inline constexpr bool operator<(Square::Offset lhs, int rhs) noexcept { return static_cast<int>(lhs) < rhs; }

template<>
inline constexpr 
auto Square::offsets<Piece::ID::King>() noexcept {
    static constexpr std::array<Square::Offset, CRAWL_NB> offsets = {{
        Square::Offset::South + Square::Offset::West,
        Square::Offset::South                       ,
        Square::Offset::South + Square::Offset::East,
        Square::Offset::West                        ,
        Square::Offset::East                        ,
        Square::Offset::North + Square::Offset::West,
        Square::Offset::North                       ,
        Square::Offset::North + Square::Offset::East,
    }};
    return offsets;
}

template<>
inline constexpr 
auto Square::offsets<Piece::ID::Knight>() noexcept {
    static constexpr std::array<Square::Offset, CRAWL_NB> offsets = {{
        Square::Offset::South + Square::Offset::South + Square::Offset::West,
        Square::Offset::South + Square::Offset::South + Square::Offset::East,
        Square::Offset::South + Square::Offset::West  + Square::Offset::West,
        Square::Offset::South + Square::Offset::East  + Square::Offset::East,
        Square::Offset::North + Square::Offset::West  + Square::Offset::West,
        Square::Offset::North + Square::Offset::East  + Square::Offset::East,
        Square::Offset::North + Square::Offset::North + Square::Offset::West,
        Square::Offset::North + Square::Offset::North + Square::Offset::East,
    }};
    return offsets;
}

template<>
inline constexpr 
auto Square::offsets<Piece::ID::Bishop>() noexcept {
    static constexpr std::array<Square::Offset, SLIDE_NB> offsets = {{
        Square::Offset::South + Square::Offset::West,
        Square::Offset::South + Square::Offset::East,
        Square::Offset::North + Square::Offset::West,
        Square::Offset::North + Square::Offset::East,
    }};
    return offsets;
}

template<>
inline constexpr 
auto Square::offsets<Piece::ID::Rook>() noexcept {
    static constexpr std::array<Square::Offset, SLIDE_NB> offsets = {{
        Square::Offset::South,
        Square::Offset::West ,
        Square::Offset::East ,
        Square::Offset::North,
    }};
    return offsets;
}

template<>
inline constexpr 
auto Square::offsets<Color::ID::Red>() noexcept {
    static constexpr std::array<Square::Offset, TAKE_NB> offsets = {{
        Square::Offset::North + Square::Offset::East ,
        Square::Offset::North + Square::Offset::West ,
    }};
    return offsets;
}

template<>
inline constexpr 
auto Square::offsets<Color::ID::Blue>() noexcept {
    static constexpr std::array<Square::Offset, TAKE_NB> offsets = {{
        Square::Offset::South + Square::Offset::East ,
        Square::Offset::North + Square::Offset::East ,
    }};
    return offsets;
}

template<>
inline constexpr 
auto Square::offsets<Color::ID::Yellow>() noexcept {
    static constexpr std::array<Square::Offset, TAKE_NB> offsets = {{
        Square::Offset::South + Square::Offset::West ,
        Square::Offset::South + Square::Offset::East ,
    }};
    return offsets;
}

template<>
inline constexpr 
auto Square::offsets<Color::ID::Green>() noexcept {
    static constexpr std::array<Square::Offset, TAKE_NB> offsets = {{
        Square::Offset::North + Square::Offset::West,
        Square::Offset::South + Square::Offset::West,
    }};
    return offsets;
}

inline constexpr 
Square::Offset Square::push(Color::ID color, int idx) noexcept { // make function for push instead of doing here
    static constexpr std::array<Square::Offset, COLOR_NB * PUSH_NB> offsets = {
        Square::Offset::North , Square::Offset::North + Square::Offset::North,
        Square::Offset::East  , Square::Offset::East  + Square::Offset::East ,
        Square::Offset::South , Square::Offset::South + Square::Offset::South,
        Square::Offset::West  , Square::Offset::West  + Square::Offset::West ,
    };
    return offsets[static_cast<std::size_t>(static_cast<uint8_t>(color) * PUSH_NB + idx)];
}

inline constexpr 
Square::Offset Square::take(Color::ID color, int idx) noexcept {
    static constexpr std::array<Square::Offset, COLOR_NB * TAKE_NB> offsets = {
        Square::Offset::North + Square::Offset::East, Square::Offset::North + Square::Offset::West,
        Square::Offset::South + Square::Offset::East, Square::Offset::North + Square::Offset::East,
        Square::Offset::South + Square::Offset::West, Square::Offset::South + Square::Offset::East,
        Square::Offset::North + Square::Offset::West, Square::Offset::South + Square::Offset::West,
    };
    return offsets[static_cast<std::size_t>(static_cast<uint8_t>(color) * TAKE_NB + idx)];
}

} // namespace athena 