#pragma once

#include <string>
#include "piece.h"
#include "color.h"

namespace athena::chess {

class PieceColor {
public:
    using ID = uint8_t;

    constexpr PieceColor() = default;
    constexpr PieceColor(ID id) noexcept : id_(id) {}
    constexpr PieceColor(Color::ID color, Piece::ID piece) 
        noexcept : id_(static_cast<ID>(
            (static_cast<uint8_t>(piece) << 3) | 
            (static_cast<uint8_t>(color)))) {}

    PieceColor(char color, char piece) noexcept {
        id_ = PieceColor(Color(color).id(), Piece(piece).id()).id_;
    }

    std::string uci() const noexcept {
        if (id_ == stone().id()) return "x";
        if (id_ == empty().id()) return "" ;
        return std::string{color().uci(), piece().uci()};
    }

    constexpr Piece piece() const noexcept { return Piece(static_cast<Piece::ID>(static_cast<int>(id_) >> 3)); }
    constexpr Color color() const noexcept { return Color(static_cast<Color::ID>(static_cast<int>(id_) &  7)); }

    constexpr ID id() const noexcept { return id_; }

private:
    ID id_;

public:
    static constexpr PieceColor stone() noexcept { return PieceColor(Color::ID::None, Piece::ID::Stone); };
    static constexpr PieceColor empty() noexcept { return PieceColor(Color::ID::None, Piece::ID::Empty); };
};

inline constexpr bool operator==(PieceColor lhs, PieceColor rhs) noexcept { return lhs.id() == rhs.id(); }
inline constexpr bool operator!=(PieceColor lhs, PieceColor rhs) noexcept { return lhs.id() != rhs.id(); }

} // namespace athena