#pragma once

#include <array>
#include <cassert>
#include "constants.h"
#include "piece.h"
#include "color.h"

namespace athena
{

class Offset
{
    public: enum : int
    {
        North = +16,
        South = -16,
        East  = +1 ,
        West  = -1 ,
        None  =  0 ,
    };

    public:

    // Default constructor
    constexpr Offset() = default;

    // Constructor
    constexpr Offset(int value)
        : value_(value) {}

    // Addition operator
    constexpr Offset operator+(Offset rhs) const {
        return Offset(value_ + rhs.value_);
    }

    // Conversion operator
    explicit constexpr operator int() const {
        return value_;
    }

    private:
    int value_;
};

inline constexpr std::array<Offset, CRAWL_NB> KNIGHT_OFFSETS =
{{
    Offset::South + Offset::South + Offset::West,
    Offset::South + Offset::South + Offset::East,
    Offset::South + Offset::West  + Offset::West,
    Offset::South + Offset::East  + Offset::East,
    Offset::North + Offset::West  + Offset::West,
    Offset::North + Offset::East  + Offset::East,
    Offset::North + Offset::North + Offset::West,
    Offset::North + Offset::North + Offset::East,
}};

inline constexpr std::array<Offset, CRAWL_NB> KING_OFFSETS = 
{{
    Offset::South + Offset::West,
    Offset::South               ,
    Offset::South + Offset::East,
    Offset::West                ,
    Offset::East                ,
    Offset::North + Offset::West,
    Offset::North               ,
    Offset::North + Offset::East,
}};

inline constexpr std::array<Offset, SLIDE_NB> BISHOP_OFFSETS =
{
    Offset::South + Offset::West,
    Offset::South + Offset::East,
    Offset::North + Offset::West,
    Offset::North + Offset::East,
};

inline constexpr std::array<Offset, SLIDE_NB> ROOK_OFFSETS = 
{
    Offset::South,
    Offset::West ,
    Offset::East ,
    Offset::North,
};

// Pawn offsets
inline constexpr std::array<std::array<Offset, PUSH_NB>, COLOR_NB> PUSH_OFFSETS =
{{
    {{ Offset::North, Offset::North + Offset::North }},
    {{ Offset::East , Offset::East  + Offset::East  }},
    {{ Offset::South, Offset::South + Offset::South }},
    {{ Offset::West , Offset::West  + Offset::West  }},
}};

// Pawn offsets
inline constexpr std::array<std::array<Offset, TAKE_NB>, COLOR_NB> TAKE_OFFSETS =
{{
    {{ Offset::North + Offset::West , Offset::North + Offset::East  }},
    {{ Offset::East  + Offset::South, Offset::East  + Offset::North }},
    {{ Offset::South + Offset::West , Offset::South + Offset::East  }},
    {{ Offset::West  + Offset::South, Offset::West  + Offset::North }},
}};

// Get crawl offsets
inline constexpr std::array<Offset, CRAWL_NB> crawl_offsets(Piece piece)
{
    assert(
    piece == Piece::Knight || 
    piece == Piece::King); // piece.crawler()

    return piece == Piece::Knight ? KNIGHT_OFFSETS : KING_OFFSETS;
}

// Get slide offsets
inline constexpr std::array<Offset, SLIDE_NB> slide_offsets(Piece piece)
{
    assert(
    piece == Piece::Bishop || 
    piece == Piece::Rook); // piece.slider()

    return piece == Piece::Bishop ? BISHOP_OFFSETS : ROOK_OFFSETS;
}

// Get push offsets
inline constexpr std::array<Offset, PUSH_NB> push_offsets(Color color)
{
    // assert(color.valid());

    return PUSH_OFFSETS[static_cast<uint8_t>(color)];
}

// Get take offsets
inline constexpr std::array<Offset, TAKE_NB> take_offsets(Color color)
{
    // assert(color.valid());

    return TAKE_OFFSETS[static_cast<uint8_t>(color)];
}

} // namespace athena