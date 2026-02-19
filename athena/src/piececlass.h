#pragma once

#include "piece.h"
#include "color.h"

namespace athena
{

class PieceClass
{
    public:

    // Default constructor
    constexpr PieceClass() = default;

    // Constructor
    constexpr PieceClass(Color color, Piece piece) noexcept 
        : encoded_((static_cast<uint8_t>(color) << 3) | static_cast<uint8_t>(piece)) {}

    // Constructor
    constexpr PieceClass(uint8_t encoded) noexcept 
        : encoded_(encoded) {}

    // UCI Constructor
    PieceClass(std::string_view pieceClass) noexcept;

    // Conversion operators
    Piece piece() const noexcept { return Piece(encoded_ &  7); }
    Color color() const noexcept { return Color(encoded_ >> 3); }

    // UCI Conversion
    std::string uci() const noexcept;

    // Conversion operator
    explicit constexpr operator uint8_t() const noexcept { return encoded_; }

    private:
    uint8_t encoded_;

    public:
    static consteval PieceClass Stone() { return PieceClass(Color::None, Piece::Stone); };
    static consteval PieceClass Empty() { return PieceClass(Color::None, Piece::Empty); };
};

// ===== operators ===== //

inline constexpr bool operator==(PieceClass lhs, PieceClass rhs) noexcept {
    return static_cast<uint8_t>(lhs) == static_cast<uint8_t>(rhs);
}

inline constexpr bool operator!=(PieceClass lhs, PieceClass rhs) noexcept {
    return static_cast<uint8_t>(lhs) != static_cast<uint8_t>(rhs);
}

} // namespace athena