#pragma once

#include <string>
#include <string_view>
#include <cstdint>

namespace athena
{

class Piece
{
    public: enum : uint8_t
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

    public:

    // Default constructor
    constexpr Piece() = default;

    // Constructor
    constexpr Piece(uint8_t value) noexcept : value_(value) {}

    // UCI Constructor
    Piece(std::string_view piece) noexcept;

    // Conversion operator
    explicit constexpr operator uint8_t() const noexcept { return value_; }

    // Return the UCI representation of the piece
    std::string uci() const noexcept;

    private:
    uint8_t value_;
};

// ===== operators ===== //

inline constexpr bool operator==(Piece lhs, Piece rhs) noexcept {
    return static_cast<uint8_t>(lhs) == static_cast<uint8_t>(rhs);
}

inline constexpr bool operator!=(Piece lhs, Piece rhs) noexcept {
    return static_cast<uint8_t>(lhs) != static_cast<uint8_t>(rhs);
}

} // namespace athena